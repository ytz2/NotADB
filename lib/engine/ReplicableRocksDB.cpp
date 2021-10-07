#include "ReplicableRocksDB.h"
#include <chrono>
#include <thread>
#include "ComparatorFactory.h"
#include "MergeOperation.h"

namespace lib {
namespace engine {

namespace {

template<class T>
T *asMsg(interface::IMessagePtr msg) {
  if (!msg) return nullptr;
  return dynamic_cast<T *>(msg.get());
}

template<class T>
const T *asConstMsg(const interface::IMessagePtr msg) {
  if (!msg) return nullptr;
  return dynamic_cast<const T *>(msg.get());
}

interface::IMessagePtr toAvroWriteMsg(const std::vector<std::string> &keys,
                                      const std::vector<std::string> &values,
                                      const std::string &col
) {
  auto msg = lib::message::MessageFactory::getInstance()->createMessageByProtocolMsgID("Avro",
                                                                                       (int) lib::message::avromsg::EMessageType::WriteMany);
  auto writeMany = asMsg<lib::message::avromsg::WriteMany>(msg);
  if (!writeMany)
    return nullptr;
  for (size_t i = 0; i < keys.size(); i++) {
    avrogen::KeyValue kv;
    kv.key = keys[i];
    kv.value = values[i];
    writeMany->keyValues.push_back(std::move(kv));
  }
  writeMany->column = col;
  return msg;
}

// mapper function, todo: move it to separate utils, only support avro now
interface::IMessagePtr toWriteMsg(const std::string &protocol, const std::vector<std::string> &keys,
                                  const std::vector<std::string> &values,
                                  const std::string &col
) {
  if (protocol == "Avro")
    return toAvroWriteMsg(keys, values, col);
  return nullptr;
}

interface::IMessagePtr toAvroPutMsg(const std::string &key,
                                    const std::string &value,
                                    const std::string &col) {
  auto msg = lib::message::MessageFactory::getInstance()->createMessageByProtocolMsgID("Avro",
                                                                                       (int) lib::message::avromsg::EMessageType::PutOne);
  auto putOne = asMsg<lib::message::avromsg::PutOne>(msg);
  if (!putOne)
    return nullptr;
  putOne->key = key;
  putOne->value = value;
  putOne->column = col;
  return msg;
}

interface::IMessagePtr toPutMsg(const std::string &protocol, const std::string &key,
                                const std::string &value,
                                const std::string &col) {
  if (protocol == "Avro")
    return toAvroPutMsg(key, value, col);
  return nullptr;
}

interface::IMessagePtr toAvroRemoveMsg(const std::string &key,
                                       const std::string &col) {
  auto msg = lib::message::MessageFactory::getInstance()->createMessageByProtocolMsgID("Avro",
                                                                                       (int) lib::message::avromsg::EMessageType::RemoveOne);
  auto removeOne = asMsg<lib::message::avromsg::RemoveOne>(msg);
  if (!removeOne)
    return nullptr;
  removeOne->key = key;
  removeOne->column = col;
  return msg;
}

interface::IMessagePtr toRemoveMsg(const std::string &protocol, const std::string &key,
                                   const std::string &col) {
  if (protocol == "Avro")
    return toAvroRemoveMsg(key, col);
  return nullptr;
}

interface::IMessagePtr toAvroRemoveRangeMsg(const std::string &col,
                                            const std::string &begin,
                                            const std::string &end
) {
  auto msg = lib::message::MessageFactory::getInstance()->createMessageByProtocolMsgID("Avro",
                                                                                       (int) lib::message::avromsg::EMessageType::RemoveRange);
  auto removeRange = asMsg<lib::message::avromsg::RemoveRange>(msg);
  if (!removeRange)
    return nullptr;
  removeRange->begin = begin;
  removeRange->end = end;
  removeRange->column = col;
  return msg;
}

interface::IMessagePtr toRemoveRangeMsg(const std::string &protocol, const std::string &col,
                                        const std::string &begin,
                                        const std::string &end
) {
  if (protocol == "Avro")
    return toAvroRemoveRangeMsg(col, begin, end);
  return nullptr;
}

}

ReplicableRocksDB::ReplicableRocksDB(config::Configuration config) {
  init(config);
}

rocksdb::Options ReplicableRocksDB::getDBOptions(config::Configuration config) {
  auto options = SimpleRocksDB::getDBOptions(config);
  config::Configuration conf;
  if (config.getConfig("comparator", conf)) {
    std::string delimStr;
    std::string comparatorStr;

    if (!conf.get("type", comparatorStr) || !conf.get("delim", delimStr) || delimStr.size() != 1) {
      throw std::runtime_error("comparator config either has no type or delim");
    }
    // a little werid to create in get function, todo: move out
    comparator_ = ComparatorFactory::createComparator(comparatorStr, delimStr[0]);
    if (!comparator_) {
      throw std::runtime_error(comparatorStr + " is not supported");
    }
    LOG(INFO) << "initialized " << name_ << " with comparator " << comparatorStr;
    options.comparator = comparator_;
    colDataOption_.comparator = comparator_;
  }
  LOG(INFO) << "DBOptions is using ReplicableRocksDB override";
  return options;
}

void ReplicableRocksDB::init(config::Configuration config) {
  // bootstrap db first
  // TODO: 1) shard id init shard id
  //       2) shard key init
  //       3) order policy init
  SimpleRocksDB::init(config);
  std::vector<config::Configuration> configs;
  if (!config.getConfigs("topics", configs)) {
    throw std::runtime_error("does not have topics in config for replicable db");
  }
  for (const auto &each: configs) {
    std::string topic, protocol;
    if (!each.get("topic", topic))
      throw std::runtime_error("no entry to topic under topics");
    if (!each.get("protocol", protocol))
      throw std::runtime_error("no entry to protocol under topics");
    producerTopics_[topic] = protocol;
  }

  lib::config::Configuration consumerConfig, producerConfig;
  if (!config.getConfig("producer", producerConfig) || !config.getConfig("consumer", consumerConfig)) {
    throw std::runtime_error("does not have producer/consumer in config for replicable db");
  }
  kakfaConsumer_ = std::make_unique<lib::kafka::Consumer>(consumerConfig);
  kakfaProducer_ = std::make_unique<lib::kafka::Producer>(producerConfig);
}

void ReplicableRocksDB::serve() {
  // start to listen
  kakfaConsumer_->registerListner(shared_from_this());
  if (!kakfaProducer_->start()) {
    throw std::runtime_error("cannot start kafka producer");
  }
  // sometimes kafka broker is flaky, let's do retry
  int count = 0;
  bool consumer_started = false;
  while (count < 3 && !consumer_started) {
    consumer_started = kakfaConsumer_->start();
    count++;
    if (consumer_started) {
      LOG(ERROR) << "cannot connect to kafka broker, retry in " << count * 5 << " seconds";
      std::this_thread::sleep_for(std::chrono::seconds(count * 5));
    }
  }
  if (!consumer_started) {
    throw std::runtime_error("cannot start kafka consumer");
  }
  LOG(INFO) << name_ << " warm up done";
}

rocksdb::Status ReplicableRocksDB::write(const std::vector<std::string> &keys,
                                         const std::vector<std::string> &values,
                                         const std::string &col
) {
  if (comparator_) {
    for (const auto &k: keys)
      if (!comparator_->validate(k))
        return rocksdb::Status::Corruption();
  }

  // create protocol msg and publish to kafka, TODO: use macro for duplicate logic
  for (auto &each: producerTopics_) {
    auto msg = toWriteMsg(each.second, keys, values, col);
    if (!msg || !kakfaProducer_->sendSync(each.first, msg, keys.front())) {
      LOG(ERROR) << "failed to write a write " << each.first << " msg for protocol" << each.second;
      return rocksdb::Status::Corruption();
    }
  }
  return rocksdb::Status::OK();
}

rocksdb::Status ReplicableRocksDB::put(const std::string &key,
                                       const std::string &value,
                                       const std::string &col
) {
  if (comparator_ && !comparator_->validate(key))
    return rocksdb::Status::Corruption(); // validate before publish
  // create protocol msg and publish to kafka
  for (auto &each: producerTopics_) {
    auto msg = toPutMsg(each.second, key, value, col);
    if (!msg || !kakfaProducer_->sendSync(each.first, msg, key)) {
      LOG(ERROR) << "failed to write a put " << each.first << " msg for protocol" << each.second;
      return rocksdb::Status::Corruption();
    }
  }
  return rocksdb::Status::OK();
}

rocksdb::Status ReplicableRocksDB::remove(const std::string &key,
                                          const std::string &col
) {
  if (comparator_ && !comparator_->validate(key))
    return rocksdb::Status::Corruption(); // validate before publish
  // create protocol msg and publish to kafka
  for (auto &each: producerTopics_) {
    auto msg = toRemoveMsg(each.second, key, col);
    if (!msg || !kakfaProducer_->sendSync(each.first, msg, key)) {
      LOG(ERROR) << "failed to write a remove " << each.first << " msg for protocol" << each.second;
      return rocksdb::Status::Corruption();
    }
  }
  return rocksdb::Status::OK();
}

rocksdb::Status ReplicableRocksDB::remove_range(const std::string &col,
                                                const std::string &begin,
                                                const std::string &end
) {
  if (comparator_ && !(comparator_->validate(begin) && comparator_->validate(end)))
    return rocksdb::Status::Corruption(); // validate before publish
  // create protocol msg and publish to kafka
  for (auto &each: producerTopics_) {
    auto msg = toRemoveRangeMsg(each.second, col, begin, end);
    if (!msg || !kakfaProducer_->sendSync(each.first, msg, begin + "-" + end)) {
      LOG(ERROR) << "failed to write a remove_range " << each.first << " msg for protocol" << each.second;
      return rocksdb::Status::Corruption();
    }
  }
  return rocksdb::Status::OK();
}

rocksdb::Status ReplicableRocksDB::merge(const std::string &key,
                                         const std::string &col,
                                         const interface::iSerializablePtr val
) {
  if (comparator_ && !comparator_->validate(key)) {
    return rocksdb::Status::Corruption(); // validate before publish
  }
  // here we strictly require the ptr to be Merge
  const auto msg = std::dynamic_pointer_cast<MergeOperation>(val);
  if (!msg) {
    LOG(ERROR) << "merge operation for replicable mode only accept Merge type";
    return rocksdb::Status::Corruption();
  }
  for (auto &each: producerTopics_) {
    if (!kakfaProducer_->sendSync(each.first, msg, key)) {
      LOG(ERROR) << "failed to write a merge " << each.first << " msg for protocol" << each.second;
      return rocksdb::Status::Corruption();
    }
  }
  return rocksdb::Status::OK();
}

// kafka callback
bool ReplicableRocksDB::shouldProcess(const interface::IMessagePtr msg) {
  return true; // no validation
}

// kafka callback
bool ReplicableRocksDB::onMessage(interface::ISessionPtr session,
                                  const interface::IMessagePtr msg) {
  if (!msg) return false;
  auto protocol = msg->GetProtocol();
  // on process avro now
  if (protocol == "Avro") {
    if (!onAvroMessage(msg)) {
      LOG(ERROR) << "failed to process msg per " << protocol;
      return false;
    }
    return true;
  }
  return false;
}

bool ReplicableRocksDB::onAvroMessage(const interface::IMessagePtr msg) {
  auto msgType = static_cast<lib::message::avromsg::EMessageType>(msg->GetMessageID());
  switch (msgType) {
    case lib::message::avromsg::EMessageType::PutOne:return onAvroPut(msg);
    case lib::message::avromsg::EMessageType::WriteMany:return onAvroWrite(msg);
    case lib::message::avromsg::EMessageType::RemoveOne:return onAvroRemove(msg);
    case lib::message::avromsg::EMessageType::RemoveRange:return onAvroRemoveRange(msg);
    case lib::message::avromsg::EMessageType::Merge: return onAvroMerge(msg);
    default:return false;
  }
  return false;
}

bool ReplicableRocksDB::onAvroMerge(const interface::IMessagePtr msg) {
  const auto mergeMsg = asConstMsg<lib::message::avromsg::Merge>(msg);
  if (!mergeMsg) return false;
  return SimpleRocksDB::merge(mergeMsg->key, mergeMsg->column, msg).ok();
}

bool ReplicableRocksDB::onAvroPut(const interface::IMessagePtr msg) {
  const auto put = asConstMsg<lib::message::avromsg::PutOne>(msg);
  if (!put) return false;
  return SimpleRocksDB::put(put->key, put->value, put->column).ok();
}

bool ReplicableRocksDB::onAvroWrite(const interface::IMessagePtr msg) {
  const auto write = asConstMsg<lib::message::avromsg::WriteMany>(msg);
  if (!write) return false;
  std::vector<std::string> keys, values;
  for (const auto &each: write->keyValues) {
    keys.push_back(each.key);
    values.push_back(each.value);
  }
  return SimpleRocksDB::write(keys, values, write->column).ok();
}

bool ReplicableRocksDB::onAvroRemove(const interface::IMessagePtr msg) {
  const auto remove = asConstMsg<lib::message::avromsg::RemoveOne>(msg);
  if (!remove) return false;
  return SimpleRocksDB::remove(remove->key, remove->column).ok();
}

bool ReplicableRocksDB::onAvroRemoveRange(const interface::IMessagePtr msg) {
  const auto removeRange = asConstMsg<lib::message::avromsg::RemoveRange>(msg);
  if (!removeRange) return false;
  return SimpleRocksDB::remove_range(removeRange->column, removeRange->begin, removeRange->end).ok();
}

}
}
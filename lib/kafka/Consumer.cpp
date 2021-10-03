#include "Consumer.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <list>
namespace lib {
namespace kafka {

Consumer::Consumer(config::Configuration config)
    : interface::ISession("KafkaConsumer") {
  init(config);
  connected_ = false;
}

Consumer::~Consumer() {
  if (consumer_)
    delete consumer_;
}

bool Consumer::start() {
  if (!consumer_)
    return false;
  if (connected_)
    return true;

  ::kafka::Topics topics;
  for (const auto &each: codecs_)
    topics.insert(each.first);
  try {
    consumer_->subscribe(topics);
    for (const auto &each: topics) {
      LOG(INFO) << name_ << " subscribed to " << each;
    }
  }
  catch (...) {
    LOG(ERROR) << "Failed to start kafka consumer";
    return false;
  }
  thread_ = std::thread([this] {
    consume();
  });
  connected_ = true;
  onConnected();
  return true;
}

void Consumer::consume() {
  LOG(INFO) << name_ << "starts to poll to consume";
  while (connected_) {
    try {
      auto records = consumer_->poll(std::chrono::milliseconds(kafkaPollTimeout_));
      for (const auto &record: records) {
        if (!record.error()) {
          handleRecord(record);
        } else {
          LOG(ERROR) << record.toString();
        }
      }
    }
    catch (const ::kafka::KafkaException &e) {
      LOG(ERROR) << "Unexpected exception caught: " << e.what();
    }
    catch (std::exception &exception) {
      LOG(ERROR) << "Unexpected exception caught: " << exception.what();
    }
    catch (...) {
      LOG(ERROR) << "Unknown exception caught";
    }
    // maybe sleep to cool down cpu here?
  }
}

void Consumer::disconnect() {
  try {
    consumer_->unsubscribe();
  }
  catch (...) {
    LOG(ERROR) << "failed to start kafka consumer";
  }
  interface::ISession::disconnect();
}

void Consumer::init(config::Configuration config) {
  // get sub configs about topics
  /*
   * topics:
   *  - topic: xyz
   *    protocol: abc
   *  - topic: abc
   *    protocol: uvw
   * kafkaPollTimeoutMs: 100
   * settings:
   *  - brokers: abc
   *  - xyz: abc
   */
  std::vector<lib::config::Configuration> configs;
  if (!config.getConfigs("topics", configs)) {
    throw std::runtime_error("no topics available");
  }
  for (const auto &each: configs) {
    std::string topic, protocol;
    if (!each.get("topic", topic))
      throw std::runtime_error("no entry to topic under topics");
    if (!each.get("protocol", protocol))
      throw std::runtime_error("no entry to protocol under topics");
    auto codec = CodecFactory::createCodec(protocol);
    if (!codec)
      throw std::runtime_error(protocol + " is not supported");
    codecs_[topic] = codec;
    LOG(INFO) << "topic " << topic << " is using " << codec->protocol() << " codec";
  }
  // get timeout
  int pollTimeout = 0;
  if (config.get("kafkaPollTimeoutMs", pollTimeout))
    kafkaPollTimeout_ = pollTimeout;
  // create consumer instance

  ::kafka::Properties props;
  // explicitly read the kafka setting
  std::vector<std::string> tokens;
  if (!config.get("settings", tokens))
    throw std::runtime_error("run time error, kafka consumer cannot find 'settings' or section is not array");
  for (const auto &line: tokens) {
    std::list<std::string> subs;
    boost::split(subs, line, boost::is_any_of(":"));
    if (subs.size() < 2)
      throw std::runtime_error("invalid input: " + line);
    auto key = subs.front();
    subs.pop_front();
    auto value = boost::join(subs, ":");
    props.put(key, value);
  }
  consumer_ = new ::kafka::KafkaConsumer(props);
}

interface::IMessagePtr Consumer::decode(const ::kafka::ConsumerRecord &record) {
  auto topic = record.topic();
  if (!codecs_.count(topic)) {
    LOG(ERROR) << topic << "does not have corresponding codec registered ";
    return nullptr;
  }
  return codecs_[topic]->deserialize(record);
}

void Consumer::handleRecord(const ::kafka::ConsumerRecord &record) {
  auto msg = decode(record);
  if (!msg) {
    LOG(ERROR) << "failed to decode record " << "Key   [" << record.key().toString() << "]"
               << "Value   [" << record.value().toString() << "]";
    return;
  }
  onMessage(msg);
}

// ICallbackContainer, hacky here
void Consumer::onMessage(const interface::IMessagePtr msg) {
  for (auto l: appCallbacks_) {
    if (l->shouldProcess(msg)) {
      l->onMessage(nullptr, msg);
    }
  }
}

}
}

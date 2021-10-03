#include "Producer.h"
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <list>
namespace lib {
namespace kafka {

const static std::size_t STRING_BUFFER_SIZE = 1024 * 1024; // reserve 1 MB for buffer

Producer::Producer(config::Configuration config)
    : interface::ISession("KafkaProducer") {
  init(config); // config is shallow copy of yaml cpp, cheap to copy
}

void Producer::init(config::Configuration config) {
  ::kafka::Properties props;
  // explicitly read the kafka setting
  std::vector<std::string> tokens;
  if (!config.get("settings", tokens))
    throw std::runtime_error("run time error, kafka producer cannot find 'settings' or section is not array");
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
  producer_ = new ::kafka::KafkaProducer(props);
  buffer_.reserve(STRING_BUFFER_SIZE);
}

Producer::~Producer() {
  if (producer_)
    delete producer_;
}

bool Producer::send(const interface::IMessagePtr msg) {
  /*not implemented*/
  return false;
}

bool Producer::start() {
  if (!producer_)
    return false;
  // nothing we can do here
  connected_ = true;
  onConnected();
  return true;
}

MessageCodecPtr Producer::getCodec(const interface::IMessagePtr msg) {
  const std::string protocol = msg->GetProtocol();
  if (!codecs_.count(protocol)) {
    codecs_[protocol] = CodecFactory::createCodec(protocol); // we don't check and put nullptr here on purpose
  }
  return codecs_[protocol];
}

bool Producer::writeToBuffer(const interface::IMessagePtr msg, std::string &buffer) {
  buffer.clear();
  auto codec = getCodec(msg);
  if (!codec) {
    LOG(ERROR) << "failed to get codec for protocol " << msg->GetProtocol();
    return false;
  }
  return codec->serialize(msg, buffer);
}

bool Producer::sendSync(const std::string &topic, const interface::IMessagePtr msg, std::string key) {
  if (!writeToBuffer(msg, buffer_)) {
    LOG(ERROR) << "Failed to write to buffer";
    return false;
  }
  auto record = ::kafka::ProducerRecord(topic,
                                        key.empty() ? ::kafka::NullKey : ::kafka::Value(key.c_str(), key.size()),
                                        ::kafka::Value(buffer_.c_str(), buffer_.size()));
  // Send the message.
  try {
    producer_->syncSend(record);
    for (auto each: this->sessionCallbacks_) {
      each->onMessageSent(shared_from_this(), msg);
    }
  } catch (const ::kafka::KafkaException &e) {
    std::cerr << "% Message delivery failed: " << e.error().message() << std::endl;
    return false;
  }
  return true;
}

bool Producer::sendAsync(const std::string &topic, const interface::IMessagePtr msg, std::string key) {
  auto buffer = std::make_shared<std::string>();
  auto keyBuffer = std::make_shared<std::string>(key);
  if (!writeToBuffer(msg, *buffer)) {
    LOG(ERROR) << "Failed to write to buffer";
    return false;
  }
  auto record = ::kafka::ProducerRecord(topic,
                                        key.empty() ? ::kafka::NullKey : ::kafka::Value(keyBuffer->c_str(),
                                                                                        keyBuffer->size()),
                                        ::kafka::Value(buffer->c_str(), buffer->size()));
  producer_->send(record,
      // lib drawback, need to capture the buffer as this is async and we do not want to destroy it
                  [keyBuffer, buffer, msg, this](const ::kafka::Producer::RecordMetadata &metadata,
                                                 const ::kafka::Error &error) {
                    if (!error) {
                      for (auto each: this->sessionCallbacks_) {
                        each->onMessageSent(shared_from_this(), msg);
                      }
                    } else {
                      LOG(ERROR) << "% Message delivery failed: " << error.message() << std::endl;
                    }
                  });
  return true;
}

}
}
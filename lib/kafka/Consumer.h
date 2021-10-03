#pragma once
#ifdef DEBUG
#undef DEBUG
#endif

#include "kafka/KafkaConsumer.h"
#include "lib/config/Configuration.h"
#include <glog/logging.h>
#include "interface/message.h"
#include "interface/connection.h"
#include "Codec.h"
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

namespace lib {
namespace kafka {
class Consumer : public interface::ISession, public std::enable_shared_from_this<Consumer> {
 public:
  Consumer() = delete;
  virtual ~Consumer();
  explicit Consumer(config::Configuration config);

  // ISession
  virtual bool send(const interface::IMessagePtr msg) override { return false; };
  virtual bool start() override;
  virtual void disconnect() override;
  void onMessage(const interface::IMessagePtr msg) override;
 protected:
  void consume();

 private:
  void init(config::Configuration config);
  void handleRecord(const ::kafka::ConsumerRecord &record);
  interface::IMessagePtr decode(const ::kafka::ConsumerRecord &record);

 private:
  ::kafka::KafkaConsumer *consumer_ = nullptr;
  std::unordered_map<std::string /*topic*/, MessageCodecPtr /*codec*/> codecs_;
  std::thread thread_;
  size_t kafkaPollTimeout_ = 100;
};
}
}

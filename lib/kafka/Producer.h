//
// Created by Danni Song on 9/12/21.
//

#ifndef DISTRIBUTED_STREAMING_LIB_KAFKA_PRODUCER_H_
#define DISTRIBUTED_STREAMING_LIB_KAFKA_PRODUCER_H_

// resolve the conflict due to modern cpp kafka conflict

#ifdef DEBUG
#undef DEBUG
#endif

#include "kafka/KafkaProducer.h"
#include <glog/logging.h>
#include "interface/message.h"
#include "interface/connection.h"
#include "lib/config/Configuration.h"
#include "Codec.h"
#include <iostream>
#include <string>

namespace lib {
namespace kafka {

class Producer : public interface::ISession {
 public:
  Producer() = delete;
  Producer(config::Configuration config);
  virtual ~Producer();

  // override
  virtual bool send(const interface::IMessagePtr msg) override;
  virtual bool start() override;

  // send to topic
  virtual bool sendSync(const std::string &topic, const interface::IMessagePtr msg, std::string key = "");

  virtual bool sendAsync(const std::string &topic, const interface::IMessagePtr msg, std::string key = "");

 protected:
  virtual void onMessage(const interface::IMessagePtr msg) override { /*noop*/}

 private:
  void init(config::Configuration config);
  MessageCodecPtr getCodec(const interface::IMessagePtr msg);
  bool writeToBuffer(const interface::IMessagePtr msg, std::string &buffer);
 private:
  ::kafka::KafkaProducer *producer_ = nullptr;
  std::unordered_map<std::string, MessageCodecPtr> codecs_;
  std::string buffer_;
};

}
}

#endif //DISTRIBUTED_STREAMING_LIB_KAFKA_PRODUCER_H_

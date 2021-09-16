#include <glog/logging.h>
#include "lib/kafka/Producer.h"
#include "lib/kafka/Consumer.h"
#include "lib/config/Configuration.h"
#include "message/commonjson/CommonJsonMessage.h"
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
const static std::string configStr = R"(
producer:
  settings:
    - bootstrap.servers:localhost:29092
    - enable.idempotence:true

consumer:
  topics:
    - topic: test
      protocol: CommonJsonMessage
  kafkaPollTimeoutMs: 100
  settings:
    - bootstrap.servers:localhost:29092
    - enable.idempotence:true
)";

class KafkaListener : public interface::IListener {
 public:
  KafkaListener() = default;
  virtual ~KafkaListener() = default;

  bool start() override { return true; }

  bool shouldProcess(const interface::IMessagePtr) override { return true; }

  virtual bool onMessage(interface::ISessionPtr session,
                         const interface::IMessagePtr msg) override {
    std::string body;
    if (msg->ToString(body)) {
      LOG(INFO) << "Received message from connection " << session->name()
                << ": " << body;
      return true;
    }
    return false;
  }
};

int main() {
  std::istringstream is(configStr);
  lib::config::Configuration root(configStr);
  lib::config::Configuration consumerConfig, producerConfig;
  if (!root.getConfig("producer", producerConfig) || !root.getConfig("consumer", consumerConfig)) {
    LOG(ERROR) << "cannot init config";
    return -1;
  }

  auto listener = std::make_shared<KafkaListener>();
  auto consumer = std::make_shared<lib::kafka::Consumer>(consumerConfig);
  auto producer =  std::make_shared<lib::kafka::Producer>(producerConfig);
  consumer->registerListner(listener);
  if (!consumer->start() && !producer->start()) {
    LOG(ERROR) << "Failed to start the consumer and producer";
    return -1;
  }
  while(1) {
    auto msg = std::make_shared<lib::message::commonjson::CommonJsonMessage>();
    auto &v = *msg;
    v["msg"] = "heartbeat";
    producer->sendSync("test", msg);
    std::this_thread::sleep_for(2000ms);
  }
  return 0;
}



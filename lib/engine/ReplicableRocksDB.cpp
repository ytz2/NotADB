#include "ReplicableRocksDB.h"
#include <chrono>
#include <thread>
namespace lib {
namespace engine {
ReplicableRocksDB::ReplicableRocksDB(config::Configuration config) {
  init(config);
}

void ReplicableRocksDB::init(config::Configuration config) {
  // bootstrap db first
  // TODO: 1) shard id init shard id
  //       2) shard key init
  //       3) order policy init
  SimpleRocksDB::init(config);
  if (!config.get("topic", kafkaTopic_)) {
    throw std::runtime_error("does not have topic in config for replicable db");
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
}

// kafka callback
bool ReplicableRocksDB::shouldProcess(const interface::IMessagePtr msg) {
  return false;
}

// kafka callback
bool ReplicableRocksDB::onMessage(interface::ISessionPtr session,
                                  const interface::IMessagePtr msg) {
  return false;
}

}
}
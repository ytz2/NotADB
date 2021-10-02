#pragma once

#include "SimpleRocksDB.h"
#include "lib/kafka/Consumer.h"
#include "lib/kafka/Producer.h"
#include "Comparator.h"
#include <unordered_map>

namespace lib {
namespace engine {
/*
 * Sample config
name: xyz
shardID: 1
dbPath: /tmp/test_rocksdb_example
dbType: Replicable
dbColumns:
  - foo
  - bar
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
    - enable.auto.commit:true
    - group.id:instance_id
# more shard strategy
# more order strategy
 */

class ReplicableRocksDB : public SimpleRocksDB,
                          public interface::IListener,
                          public std::enable_shared_from_this<ReplicableRocksDB> {
 public:
  ReplicableRocksDB() = delete;
  explicit ReplicableRocksDB(config::Configuration config);
  virtual ~ReplicableRocksDB() = default;
  virtual void serve() override;
 public: // IListener
  bool start() override { return true; }
  virtual bool shouldProcess(const interface::IMessagePtr msg) override;
  virtual bool onMessage(interface::ISessionPtr session,
                         const interface::IMessagePtr msg) override;
 protected:
  virtual void init(config::Configuration config) override;
  virtual rocksdb::Options getDBOptions(config::Configuration config) override;
 private:
  std::unordered_map<std::string /*topic*/, lib::kafka::MessageCodecPtr /*codec*/> codecs_;
  std::unique_ptr<lib::kafka::Consumer> kakfaConsumer_ = nullptr;
  std::unique_ptr<lib::kafka::Producer> kakfaProducer_ = nullptr;
  Comparator *comparator_ = nullptr;
};
}
}

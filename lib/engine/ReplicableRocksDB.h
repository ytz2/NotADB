#pragma once

#include "SimpleRocksDB.h"
#include "lib/kafka/Consumer.h"
#include "lib/kafka/Producer.h"
#include "message/MessageFactory.h"
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
 public: // override the write operations to make it async
  virtual rocksdb::Status write(const std::vector<std::string> &keys,
                                const std::vector<std::string> &values,
                                const std::string &col
  ) override;

  virtual rocksdb::Status put(const std::string &key,
                              const std::string &value,
                              const std::string &col
  ) override;

  virtual rocksdb::Status remove(const std::string &key,
                                 const std::string &col
  ) override;

  virtual rocksdb::Status remove_range(const std::string &col,
                                       const std::string &begin,
                                       const std::string &end
  ) override;

  virtual rocksdb::Status merge(const std::string &key,
                                const std::string &col,
                                const interface::iSerializablePtr val
  ) override;

 protected:
  virtual void init(config::Configuration config) override;
  virtual rocksdb::Options getDBOptions(config::Configuration config) override;

 protected:
  bool onAvroMessage(const interface::IMessagePtr msg);
  bool onAvroPut(const interface::IMessagePtr msg);
  bool onAvroWrite(const interface::IMessagePtr msg);
  bool onAvroRemove(const interface::IMessagePtr msg);
  bool onAvroRemoveRange(const interface::IMessagePtr msg);
  bool onAvroMerge(const interface::IMessagePtr msg);

 private:
  std::unordered_map<std::string /*topic*/, std::string /*protocols*/> producerTopics_;
  std::unique_ptr<lib::kafka::Consumer> kakfaConsumer_ = nullptr;
  std::unique_ptr<lib::kafka::Producer> kakfaProducer_ = nullptr;
  Comparator *comparator_ = nullptr;
};
}
}

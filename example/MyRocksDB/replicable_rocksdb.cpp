#include "lib/engine/RocksDBFactory.h"
#include <string>
#include <cassert>
#include <glog/logging.h>
#include <chrono>
#include <thread>

#define dbcall(call, exp, ...) assert(call(__VA_ARGS__).ok() == exp);

int main() {
  google::InstallFailureSignalHandler();
  const static std::string configStr = R"(
dbName: test
dbPath: /tmp/test_rocksdb_example
dbType: Replicable
dbColumns:
  - foo
  - bar
topics:
  - topic: test
    protocol: Avro
comparator:
  type: KeyValue
  delim: "-"
producer:
  settings:
    - bootstrap.servers:localhost:29092
    - enable.idempotence:true
consumer:
  topics:
    - topic: test
      protocol: Avro
  kafkaPollTimeoutMs: 100
  settings:
    - bootstrap.servers:localhost:29092
    - enable.auto.commit:true
    - group.id:1
)";
  std::istringstream is(configStr);
  lib::config::Configuration config(is);
  try {
    auto db = lib::engine::CreateDB(config);
    db->serve();
    int counter = 0;
    while (true) {
      counter++;
      std::string val = std::to_string(counter);
      std::string key = "key-" + val;
      LOG(INFO) << "put " << key <<": " << val;
      if (db->put(key, val, "foo").ok())
        LOG(INFO) << "success";
      else
        LOG(ERROR) <<"fail";
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      std::string val2;
      LOG(INFO) << "read the key " << key;
      if (db->get(key, val2, "foo").ok()) {
        LOG(INFO) << "get " << key <<": " << val2;
      } else {
        LOG(ERROR) <<"fail";
      }
    }
  } catch (std::exception &e) {
    LOG(ERROR) << e.what();
  }
  return 0;
}
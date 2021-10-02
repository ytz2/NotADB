#include "lib/engine/RocksDBFactory.h"
#include <string>
#include <cassert>
#include <glog/logging.h>
#include <chrono>
#include <thread>

#define dbcall(call, exp, ...) assert(call(__VA_ARGS__).ok() == exp);

int main() {
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
    while (true)
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  } catch (std::exception &e) {
    LOG(ERROR) << e.what();
  }
  return 0;
}
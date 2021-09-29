#include "lib/engine/RocksDBFactory.h"
#include <string>
#include <cassert>
#include <glog/logging.h>

#define dbcall(call, exp, ...) assert(call(__VA_ARGS__).ok() == exp);

int main() {
  const static std::string configStr = R"(
dbPath: /tmp/test_rocksdb_example
dbType: SimpleKV
dbColumns:
  - foo
  - bar
)";
  std::istringstream is(configStr);
  lib::config::Configuration config(is);
  try {
    auto db = lib::engine::CreateDB(config);
    dbcall(db->put, true, "test", "1", "foo");
    dbcall(db->put, true, "test2", "2", "foo");
    std::string val;
    dbcall(db->get, true, "test", val, "foo");
    assert(val == "1");
    dbcall(db->remove, true, "test2", "foo");
    dbcall(db->get, false, "test2", val, "foo");
    LOG(INFO) << "Done";
  } catch (std::exception &e) {
    LOG(ERROR) << e.what();
  }
  return 0;
}
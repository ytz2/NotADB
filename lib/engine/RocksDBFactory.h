#pragma once
#include "SimpleRocksDB.h"
#include "ReplicableRocksDB.h"
namespace lib {
namespace engine {
interface::iRocksDBPtr CreateDB(config::Configuration config) {
  std::string type;
  if (!config.get("dbType", type)) {
    throw std::runtime_error("config does not contains dbType");
  }
  if (type == "SimpleKV")
    return std::make_shared<SimpleRocksDB>(config);
  if (type == "AsyncKV")
    return std::make_shared<ReplicableRocksDB>(config);
  return nullptr;
}
}
}

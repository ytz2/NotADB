#pragma once
#include "RocksDBFactory.h"
#include <map>

namespace lib {
namespace engine {

// use this one to enforce singleton of db instance on app level
class RocksDBManager {
 public:
  RocksDBManager& getInstance() {
    static RocksDBManager;
    return RocksDBManager;
  }

  bool initialize(config::Configuration config) {
    config_ = config.clone();
  }

  interface::iRocksDBPtr getDB(const std::string& name) {
    if (!dbs_.count(name)) {
      config::Configuration conf;
      if (!config.getConfig(name, conf)) {
        throw std::runtime_error("config does not contains " + name);
      }
      auto db = CreateDB(conf);
      if (!db)
        return nullptr;
      dbs_[name] = db;
    }
    return dbs_[name];
  }

 private:
  config::Configuration config_;
  std::map<std::string, interface::iRocksDBPtr> dbs_;
 private:
  RocksDBManager() = default;
};
}
}

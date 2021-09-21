//
// Created by Yanhua Liu on 9/20/21.
//

#ifndef NOTADB_LIB_ENGINE_ROCKSDBFACTORY_H_
#define NOTADB_LIB_ENGINE_ROCKSDBFACTORY_H_
#include "SimpleRocksDB.h"

namespace lib {
namespace engine {
interface::iRocksDBPtr CreateDB(config::Configuration config) {
  std::string type;
  if (!config.get("dbType", type)) {
    throw std::runtime_error("config does not contains dbType");
  }
  if (type == "SimpleKV")
    return std::make_shared<SimpleRocksDB>(config);
  return nullptr;
}
}
}

#endif //NOTADB_LIB_ENGINE_ROCKSDBFACTORY_H_

#pragma once

#include "SimpleRocksDB.h"

namespace lib {
namespace engine {
class AsyncRocksDB : public SimpleRocksDB{
 public:
  AsyncRocksDB() = delete;
  explicit AsyncRocksDB(config::Configuration config);
  virtual ~AsyncRocksDB() = default;
 protected:
  virtual void init(config::Configuration config) override;
};
}
}

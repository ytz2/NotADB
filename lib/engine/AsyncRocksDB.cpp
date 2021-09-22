#include "AsyncRocksDB.h"

namespace lib {
namespace engine {
AsyncRocksDB::AsyncRocksDB(config::Configuration config)
  :SimpleRocksDB(config)
{
}

void AsyncRocksDB::init(config::Configuration config) {
   // add kafka here
}
}
}
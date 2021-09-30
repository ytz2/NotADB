#pragma  once
#include "rocksdb/db.h"
#include "rocksdb/comparator.h"
#include "rocksdb/slice.h"
#include <string>

namespace lib {
namespace engine {
namespace comparator {

class Comparator : public rocksdb::Comparator {
 public:
  virtual bool validate(const std::string &input) const = 0;
};
}
}
}
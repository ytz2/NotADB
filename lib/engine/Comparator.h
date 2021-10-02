#pragma  once
#include "rocksdb/db.h"
#include "rocksdb/comparator.h"
#include "rocksdb/slice.h"
#include <string>
#include <cctype>
#include <boost/lexical_cast.hpp>

namespace lib {
namespace engine {

class ComparatorUtil {
 public:
  static bool validateKVValue(const std::string &input, char delim) {
    // sanity check just in case wrong key is written into db
    // 1. it must have two delim
    auto first = input.find(delim);
    if (first == std::string::npos || input.size() == first + 1)
      return false;
    // 2. verify the first char after the first delim is numerical, simple check
    if (!std::isdigit(input.at(first + 1)))
      return false;
    return true;
  }
};

class Comparator : public rocksdb::Comparator {
 public:
  virtual bool validate(const std::string &input) const = 0;
};

}
}
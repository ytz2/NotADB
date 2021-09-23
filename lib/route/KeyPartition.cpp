#include "KeyPartition.h"

namespace lib {
namespace route {
namespace partition {

std::string KeyPartition::getPartitionKey(const std::string &key) {
  return key;
}

}
}
}
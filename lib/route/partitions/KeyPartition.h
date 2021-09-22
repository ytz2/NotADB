#pragma once
#include "interface/partition.h"
namespace lib {
namespace route {
namespace partition {
class KeyPartition : public interface::IPartitionStrategy {
 public:
  KeyPartition() = default;
  virtual ~KeyPartition() = default;
  virtual std::string getPartitionKey(const std::string &key) override;
};
}
}
}

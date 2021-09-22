#pragma once
#include <string>
#include <memory>

namespace interface {

class IPartitionStrategy {
 public:
  IPartitionStrategy() = default;
  virtual ~IPartitionStrategy() = default;
  // we need this one thread safe
  virtual std::string getPartitionKey(const std::string &key) = 0;
};

typedef std::shared_ptr<IPartitionStrategy> IPartitionStrategyPtr;

}
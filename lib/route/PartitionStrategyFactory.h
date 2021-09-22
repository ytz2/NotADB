#pragma once

#include "interface/partition.h"
#include "lib/config/Configuration.h"

namespace lib {
namespace route {

class PartitionStrategyFactory {
 public:
  static interface::IPartitionStrategyPtr CreateStrategy(config::Configuration config);
};

}
}
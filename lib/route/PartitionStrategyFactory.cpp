#include "PartitionStrategyFactory.h"
#include "lib/route/partitions/PrefixPartition.h"
#include "lib/route/partitions/KeyPartition.h"
#include <string>
#include <exception>

namespace lib {
namespace route {

interface::IPartitionStrategyPtr PartitionStrategyFactory::CreateStrategy(config::Configuration config) {
  std::string strat;
  if (!config.get("partitionStrategy", strat)) {
    LOG(ERROR) << "missing " << "partitionStrategy and default to key partition";
    return std::make_shared<partition::KeyPartition>();
  }
  if (strat == "keyPartition") {
    return std::make_shared<partition::KeyPartition>();
  }
  if (strat == "prefixPartition") {
    config::Configuration prefixCong;
    if (!config.getConfig("prefixPartition", prefixCong)) {
      LOG(ERROR) << "missing " << "prefixPartition config";
      throw std::runtime_error("missing prefixPartition config");
      return std::make_shared<partition::PrefixPartition>(prefixCong);
    }
  }
  LOG(ERROR) << strat << "is not supported";
  throw std::runtime_error(strat + " is not supported");
  return nullptr;
}

}
}
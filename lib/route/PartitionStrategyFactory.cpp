#include "PartitionStrategyFactory.h"
#include "PrefixPartition.h"
#include "KeyPartition.h"
#include <string>
#include <exception>

namespace lib {
namespace route {
/**
 *
 * @param config
 * @return strategy
 * example:
 * partitionStrategy: keyPartition
 * or:
 * partitionStrategy: prefixPartition
 * prefixPartition:
 *  type: ByDelim
 *  delim: |
 * or
 * prefixPartition:
 *  type: ByFirstN
 *  delim: 10
 */

interface::IPartitionStrategyPtr PartitionStrategyFactory::CreateStrategy(config::Configuration config) {
  std::string strat;
  if (!config.get("partitionStrategy", strat)) {
    LOG(ERROR) << "missing " << "partitionStrategy and default to key partition";
    return std::make_shared<partition::KeyPartition>();
  }
  if (strat == "keyPartition") {
    LOG(INFO) << "using keyPartition strategy";
    return std::make_shared<partition::KeyPartition>();
  }
  if (strat == "prefixPartition") {
    config::Configuration prefixCong;
    if (!config.getConfig("prefixPartition", prefixCong)) {
      LOG(ERROR) << "missing " << "prefixPartition config";
      throw std::runtime_error("missing prefixPartition config");
    }
    LOG(INFO) << "using prefixPartition strategy";
    return std::make_shared<partition::PrefixPartition>(prefixCong);
  }
  LOG(ERROR) << strat << "is not supported";
  throw std::runtime_error(strat + " is not supported");
  return nullptr;
}

}
}
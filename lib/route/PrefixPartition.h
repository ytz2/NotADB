#pragma once
#include "interface/partition.h"
#include "lib/config/Configuration.h"

namespace lib {
namespace route {
namespace partition {
class PrefixPartition : public interface::IPartitionStrategy {
 public:
  PrefixPartition() = delete;
  PrefixPartition(config::Configuration config);
  virtual ~PrefixPartition() = default;
  virtual std::string getPartitionKey(const std::string &key) override;
 private:
  std::string fistN(const std::string &k);
  std::string delim(const std::string &k);
 private:
  enum EPrefixType {
    BY_DELIM = 0,
    BY_FIRST_N = 1,
    MAX_NUM_TYPE = 2
  };
  char delim_ = '|'; // default delimiter
  int charCounts_ = 8; // default char counter in the space of 2^8
  EPrefixType prefixType_ = BY_DELIM;
};
}
}
}
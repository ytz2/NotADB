#include "PrefixPartition.h"
#include <glog/logging.h>
#include <stdexcept>

namespace lib {
namespace route {
namespace partition {

PrefixPartition::PrefixPartition(config::Configuration config) {
  std::string type, value;
  if (!config.get("type", type)) {
    LOG(ERROR) << "does not contains " << "type token";
    throw std::runtime_error("failed to create PrefixPartition due to missing config type");
  }
  if (type == "ByDelim") {
    prefixType_ = BY_DELIM;
    if (!config.get("delim", value)) {
      LOG(ERROR) << "does not contains " << "delim token";
      throw std::runtime_error("failed to create PrefixPartition due to missing config delim");
    }
    delim_ = value[0];
  } else if (type == "ByFirstN") {
    prefixType_ = BY_FIRST_N;
    if (!config.get("firstN", value)) {
      LOG(ERROR) << "does not contains " << "firstN token";
      throw std::runtime_error("failed to create PrefixPartition due to missing config firstN");
    }
    try {
      charCounts_ = std::stoi(value);
    } catch (std::exception &e) {
      LOG(ERROR) << "cannot convert value " << value << "to valid number";
      throw e;
    }
  } else {
    throw std::runtime_error("failed to create PrefixPartition due to invalid config type " + type);
  }
}

std::string PrefixPartition::getPartitionKey(const std::string &key) {
  switch (prefixType_) {
    case BY_DELIM:return delim(key);
    case BY_FIRST_N:return delim(key);
    default:return key; // should not happen
  }
  return key;
}

std::string PrefixPartition::fistN(const std::string &k) {
  if (k.size() < (size_t) charCounts_)
    return k;
  return k.substr(0, charCounts_);
}

std::string PrefixPartition::delim(const std::string &k) {
  auto p = k.find(delim_);
  if (p == std::string::npos)
    return k;
  return k.substr(0, p);
}

}
}
}
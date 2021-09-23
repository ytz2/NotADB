#include "Router.h"
#include <vector>
#include <functional>

namespace lib {
namespace route {

namespace {
int32_t
JumpConsistentHash(uint64_t key, int32_t num_buckets) {
  int64_t b = 1, j = 0;
  while (j < num_buckets) {
    b = j;
    key = key * 2862933555777941757ULL + 1;
    j = (b + 1) * (double(1LL << 31) / double((key >> 33) + 1));
  }
  return b;
}
}

Router::Router(config::Configuration config) {
  LOG(INFO) << "initializing router";
  partitionExtractor_ = PartitionStrategyFactory::CreateStrategy(config);
  std::vector<config::Configuration> configs;
  if (!config.getConfigs("nodes", configs) || configs.empty()) {
    throw std::runtime_error("config does not contain array of nodes definitions");
  }
  for (const auto &each: configs) {
    std::string name, addr;
    int port;
    if (!each.get("name", name) || !each.get("address", addr) || !each.get("port", port)) {
      throw std::runtime_error("configs must have name, address and port");
    }
    nodes_.push_back(std::make_shared<Node>(name, addr, port));
    LOG(INFO) << "router added " << name;
  }
  LOG(INFO) << "router initialized";
}

Router::~Router() {
  // noop
}

const NodePtr Router::getDestination(const std::string &key) {
  auto k = partitionExtractor_->getPartitionKey(key);
  uint64_t knum = std::hash<std::string>{}(k);
  auto ind = JumpConsistentHash(knum, nodes_.size());
  if (ind >= nodes_.size())
    throw std::runtime_error(key + " cannot allocate vnode");
  return nodes_[ind];
}

}
}
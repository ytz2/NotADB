#pragma once
#include <vector>
#include <string>
#include "PartitionStrategyFactory.h"
#include "lib/config/Configuration.h"

namespace lib {
namespace route {

struct Node {
  Node(const std::string &name, const std::string &address, int port)
      : name_(name), address_(address), port_(port) {

  }
  virtual ~Node() = default;

  const std::string &name() {
    return name_;
  }

  const std::string &address() {
    return address_;
  }

  const int port() {
    return port_;
  }

  std::string name_;
  std::string address_;
  int port_;
};

typedef std::shared_ptr<Node> NodePtr;

class Router {
 public:
  Router() = delete;
  virtual ~Router();
  /**
   *
   * @param config
   * eg:
   * numReplica: 1
   * partitionStrategy: prefixPartition
   * prefixPartition:
   *  type: ByDelim
   *  delim: |
   * nodes:
   *    - name: hello
   *      address: localhost
   *      port: 23021
   *    - name: hello2
   *      address: localhost
   *      port: 23021
   */
  Router(config::Configuration config);

 public:
  const NodePtr getDestination(const std::string &key);

 private:
  std::vector<NodePtr> nodes_;
  interface::IPartitionStrategyPtr partitionExtractor_;
};
}
}


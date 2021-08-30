//
// Created by Yanhua Liu on 8/24/21.
//

#ifndef DISTRIBUTED_STREAMING_CONFIGURATION_H
#define DISTRIBUTED_STREAMING_CONFIGURATION_H

#include <string>
#include <fstream>
#include <glog/logging.h>
#include <yaml-cpp/yaml.h>
#include <exception>
#include <memory>

namespace lib {
namespace config {

// a simple wrapper on top of YAML-cpp in case I change mind
class Configuration {
  typedef YAML::Node Node;
  typedef YAML::iterator Iterator;
  typedef std::shared_ptr<Node> NodePtr;
 public:
  Configuration(const std::string &path);
  Configuration(const Node &node);

  virtual ~Configuration() {};

  // user needs to provides >> operators
  template<typename T>
  bool get(const std::string &path, T &data) const {
    Node node;
    if (!find(path, node)) {
      LOG(ERROR) << path << "does not exist";
      return false;
    }
    try {
      node >> data;
    }
    catch (YAML::ParserException &e) {
      LOG(ERROR) << path << "does not exist";
      return false;
    }
    catch (std::exception &e) {
      LOG(ERROR) << "failed to deserialize due to " << e.what();
      return false;
    }
    catch (...) {
      LOG(ERROR) << "unexpected exception while parsing config";
      return false;
    }
    return true;
  }

  bool getConfig(const std::string &path, Configuration &config) const;

 private:
  bool find(const std::string &path, Node &node) const;

 protected:
  Node node_;
};

}
}

#endif

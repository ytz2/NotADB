#pragma once

#include <string>
#include <fstream>
#include <glog/logging.h>
#include <yaml-cpp/yaml.h>
#include <exception>
#include <memory>
#include <iostream>
#include <list>

namespace lib {
namespace config {

// a simple wrapper on top of YAML-cpp in case I change mind
class Configuration {
  typedef YAML::Node Node;
  typedef YAML::iterator Iterator;
  typedef std::shared_ptr<Node> NodePtr;
 public:
  Configuration() = default;
  Configuration(std::istream &input);
  Configuration(const std::string &path);
  Configuration(const Node &node);

  Configuration clone(const Configuration& rh){
    return Configuration(rh.node_);
  }

  virtual ~Configuration() {};

  // user needs to provides >> operators
  template<typename T>
  bool get(const std::string &path, T &data) const {
    Configuration::Node node;
    node.reset(find(path));
    if (!node) {
      LOG(ERROR) << path << " does not exist";
      return false;
    }
    try {
      data = node.as<T>();
    }
    catch (YAML::ParserException &e) {
      LOG(ERROR) << path << " cannot be parsed: " << e.what();
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
  bool getConfigs(const std::string &path, std::vector<Configuration> &configs) const;

 private:
  Node find(const std::string &path) const;

 protected:
  Node node_;
};

}
}

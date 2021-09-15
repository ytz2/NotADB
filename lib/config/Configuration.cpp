//
// Created by Yanhua Liu on 8/24/21.
//

#include "Configuration.h"
#include <boost/algorithm/string.hpp>
#include <vector>

namespace lib {
namespace config {

Configuration::Configuration(std::istream &input) {
  try {
    node_ = YAML::Load(input);
  } catch (YAML::ParserException &e) {
    throw std::runtime_error(e.what());
  } catch (...) {
    throw std::runtime_error("unknown error");
  }
}

Configuration::Configuration(const std::string &path) {
  try {
    node_ = YAML::LoadFile(path);
    // do stuff
  } catch (YAML::ParserException &e) {
    throw std::runtime_error(e.what());
  } catch (...) {
    throw std::runtime_error("unknown error");
  }
}

Configuration::Configuration(const Node &node)
    : node_(node) {
}

Configuration::Node Configuration::find(const std::string &path) const {
  Configuration::Node tmp;
  tmp.reset(node_);

  if (path.empty()) return tmp;
  std::list<std::string> tokens;
  boost::split(tokens, path, boost::is_any_of("\t.\\,|"));

  for (const auto &each: tokens) {
    tmp.reset(tmp[each]);
    if (!tmp)
      return tmp;
  }
  return tmp;
}

bool Configuration::getConfig(const std::string &path, Configuration &config) const {
  Configuration::Node node;
  node.reset(find(path));
  if (!node)
    return false;
  config = Configuration(node);
  return true;
}

bool Configuration::getConfigs(const std::string &path, std::vector<Configuration> &configs) const {
  Configuration::Node node;
  node.reset(find(path));
  if (!node || !node.IsSequence())
    return false;
  for (size_t i = 0; i < node.size(); i++) {
    Configuration::Node tmp;
    tmp.reset(node[i]);
    configs.push_back(Configuration(tmp));
  }
  return true;
}

}
}

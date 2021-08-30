//
// Created by Yanhua Liu on 8/24/21.
//

#include "Configuration.h"
#include <boost/algorithm/string.hpp>
#include <vector>

namespace lib {
namespace config {

Configuration::Configuration(const std::string &path) {
  try {
    node_ = YAML::Load(path);
    // do stuff
  } catch (YAML::ParserException &e) {
    throw std::runtime_error(e.what());
  } catch (...) {
    throw std::runtime_error("unknown error");
  }
}

Configuration::Configuration(const Node &node)
    : node_(node) {}

bool Configuration::find(const std::string &path, Configuration::Node &node) const {
  node = node_;
  std::vector<std::string> tokens;
  boost::split(tokens, path, boost::is_any_of("\t.\\,|"));
  for (const auto &each : tokens) {
    if (node[each])
      node = node[each];
    else
      return false;
  }
  return true;
}

bool Configuration::getConfig(const std::string &path, Configuration &config) const {
  Configuration::Node node;
  if (!find(path, node))
    return false;
  config = Configuration(node);
  return true;
}
}
}

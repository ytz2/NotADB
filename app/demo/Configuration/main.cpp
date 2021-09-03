//
// Created by Yanhua Liu on 8/31/21.
//

#include "lib/config/Configuration.h"
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>

void getVal(const lib::config::Configuration &conf, std::string path) {
  std::string token;
  if (!conf.get(path, token)) {
    return;
  } else {
    LOG(INFO) << path << "=>" << token;
  }
}

void getVecs(const lib::config::Configuration &conf, std::string path) {
  std::vector<std::string> tokens;
  if (!conf.get(path, tokens)) {
    return;
  } else {
    LOG(INFO) << path << "=>" << boost::algorithm::join(tokens, ",");
  }
}

int main() {
  std::string example = R"(
development:
  database: myapp_development
  adapter:  postgres
  host:     localhost
  ports:
    - 123
    - 124
    - 125

test:
  database: myapp_development
  adapter:  mysql
  host:     localhost
  ports:
    - 321

)";
  std::istringstream is(example);

  lib::config::Configuration root(is);

  {
    getVal(root, "development.adapter");
    getVal(root, "development.database");
    getVal(root, "development.host");
    getVecs(root, "development.ports");
    getVal(root, "test.adapter");
    getVal(root, "test.database");
    getVal(root, "test.host");
    getVecs(root, "test.ports");
  }

  {
    lib::config::Configuration dev;
    if (!root.getConfig("development", dev))
      return -1;
    getVal(dev, "adapter");
    getVal(dev, "database");
    getVal(dev, "host");
    getVecs(dev, "ports");
  }

  return 0;
}
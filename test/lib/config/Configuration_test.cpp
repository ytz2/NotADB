#include "gtest/gtest.h"
#include "lib/config/Configuration.h"
#include <string>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>

const static std::string example = R"(
development: &base
  database: myapp_development
  adapter:  postgres
  host:     localhost
  ports:
    - 123
    - 124
    - 125

production: *base

test:
  database: myapp_test
  adapter:  mysql
  host:     localhost
  ports:
    - 321
  share:
    - shares: -75.088
      date: 11/27/2015
    - shares: 75.088
      date: 11/26/2015
)";

TEST(ConfigurationTest, getVal) {
  std::istringstream is(example);
  lib::config::Configuration root(is);
  std::string token;
  EXPECT_TRUE(root.get("development.database", token) && token == "myapp_development");
  EXPECT_TRUE(root.get("test.database", token) && token == "myapp_test");
  std::vector<std::string> tokens;
  EXPECT_TRUE(root.get("development.ports", tokens) && tokens.size() == 3 && boost::join(tokens, ",") == "123,124,125");
  EXPECT_TRUE(root.get("test.ports", tokens) && tokens.size() == 1 && boost::join(tokens, ",") == "321");
  EXPECT_FALSE(root.get("hello", token));
  EXPECT_FALSE(root.get("development.ports", token));
  tokens.clear();
  EXPECT_TRUE(root.get("production.ports", tokens) && tokens.size() == 3 && boost::join(tokens, ",") == "123,124,125");
}

TEST(ConfigurationTest, getConfig) {
  std::istringstream is(example);
  lib::config::Configuration root(is);
  lib::config::Configuration dev;
  std::string token;
  EXPECT_TRUE(root.getConfig("development", dev));
  EXPECT_TRUE(dev.get("host", token) && token == "localhost");
}

TEST(ConfigurationTest, getConfigs) {
  std::istringstream is(example);
  lib::config::Configuration root(is);
  std::vector<lib::config::Configuration> configs;
  EXPECT_TRUE(root.getConfigs("test.share", configs) && configs.size() == 2);
  std::string token; EXPECT_TRUE(configs.front().get("shares", token) && token == "-75.088");
  EXPECT_TRUE(configs.back().get("date", token) && token == "11/26/2015");
}
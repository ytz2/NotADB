#include <string>
#include <sstream>
#include <vector>
#include "gtest/gtest.h"
#include "lib/route/Router.h"
#include "lib/config/Configuration.h"


const static std::string example1 = R"(
numReplica: 1
partitionStrategy: keyPartition
nodes:
  - name: hello
    address: localhost
    port: 23021
  - name: hello2
    address: localhost
    port: 23021
)";

const static std::string example2 = R"(
numReplica: 1
partitionStrategy: prefixPartition
prefixPartition:
  type: ByDelim
  delim: |
nodes:
  - name: hello
    address: localhost
    port: 23021
  - name: hello2
    address: localhost
    port: 23021
)";

const static std::string example3 = R"(
numReplica: 1
partitionStrategy: prefixPartition
prefixPartition:
  type: ByFirstN
  firstN: 5
nodes:
  - name: hello
    address: localhost
    port: 23021
  - name: hello2
    address: localhost
    port: 23021
)";

TEST(ConfigurationTest, keyPartition) {
  std::istringstream is(example1);
  lib::config::Configuration config(is);
  std::string token;
  lib::route::Router router(config);
  EXPECT_TRUE(router.getDestination("test1") != nullptr);
}

TEST(ConfigurationTest, prefixPartitionDelim) {
  std::istringstream is(example2);
  lib::config::Configuration config(is);
  std::string token;
lib::route::Router router(config);
  EXPECT_TRUE(router.getDestination("test2") != nullptr);
}

TEST(ConfigurationTest, prefixPartitionFirstN) {
  std::istringstream is(example3);
  lib::config::Configuration config(is);
  std::string token;
  lib::route::Router router(config);
  EXPECT_TRUE(router.getDestination("test3") != nullptr);
}

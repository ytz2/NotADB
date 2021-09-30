#include <string>
#include "gtest/gtest.h"
#include "lib/engine/PrefixScoreObject.h"
#include <iostream>

TEST(PrefixScoreObject, parse) {
  lib::engine::comparator::PrefixScoreObject cmp('-');
  std::string key, objID;
  int64_t score;
  rocksdb::Slice input = "test-1234-obj1";
  cmp.parse(input, key, score, objID);
  ASSERT_STREQ(key.c_str(), "test");
  ASSERT_STREQ(objID.c_str(), "obj1");
  ASSERT_EQ(score, 1234);
}

TEST(PrefixScoreObject, parseHist) {
  lib::engine::comparator::PrefixScoreObject cmp('-');
  std::string key, objID;
  int64_t score;
  rocksdb::Slice input = "test-1234";
  cmp.parse(input, key, score, objID);
  ASSERT_STREQ(key.c_str(), "test");
  ASSERT_STREQ(objID.c_str(), "");
  ASSERT_EQ(score, 1234);
}

TEST(PrefixScoreObject, validate) {
  lib::engine::comparator::PrefixScoreObject cmp('-');
  ASSERT_TRUE(cmp.validate("test-1234"));
  ASSERT_TRUE(cmp.validate("test-1234-test"));
  ASSERT_FALSE(cmp.validate("test"));
  ASSERT_FALSE(cmp.validate("test--"));
}

TEST(PrefixScoreObject, compare) {
  lib::engine::comparator::PrefixScoreObject cmp('-');
  {
    rocksdb::Slice input1 = "test-1234-obj1";
    rocksdb::Slice input2 = "test-1234-obj1";
    ASSERT_EQ( cmp.Compare(input1, input2), 0);
  }
  {
    rocksdb::Slice input1 = "test1-1234-obj1";
    rocksdb::Slice input2 = "test2-1234-obj1";
    ASSERT_EQ( cmp.Compare(input1, input2), -1);
    ASSERT_EQ( cmp.Compare(input2, input1), 1);
  }
  {
    rocksdb::Slice input1 = "test1-1234-obj1";
    rocksdb::Slice input2 = "test1-1235-obj1";
    ASSERT_EQ( cmp.Compare(input1, input2), -1);
    ASSERT_EQ( cmp.Compare(input2, input1), 1);
  }
  {
    rocksdb::Slice input1 = "test1-1234-obj1";
    rocksdb::Slice input2 = "test1-1234-obj2";
    ASSERT_EQ( cmp.Compare(input1, input2), -1);
    ASSERT_EQ( cmp.Compare(input2, input1), 1);
  }
  {
    rocksdb::Slice input1 = "test1-1234";
    rocksdb::Slice input2 = "test1-1235";
    ASSERT_EQ( cmp.Compare(input1, input2), -1);
    ASSERT_EQ( cmp.Compare(input2, input1), 1);
  }
}
#include "PrefixScoreObject.h"
#include <cctype>
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <glog/logging.h>

namespace lib {
namespace engine {
namespace comparator {

PrefixScoreObject::PrefixScoreObject(char delim)
    : delim_(delim) {}

bool PrefixScoreObject::validate(const std::string &input) const {
  // sanity check just in case wrong key is written into db
  // 1. it must have two delim
  auto first = input.find(delim_);
  if (first == std::string::npos || input.size() == first + 1)
    return false;
  // 2. verify the first char after the first delim is numerical, simple check
  if (!std::isdigit(input.at(first + 1)))
    return false;
  return true;
}
/*
 *  prefix 1
 *    score: 0. 9 object id
 *    score 0.9 object id
 */

int PrefixScoreObject::Compare(const rocksdb::Slice &a, const rocksdb::Slice &b) const {
  std::string prefix1, object1, prefix2, object2;
  int64_t score1, score2;
  parse(a, prefix1, score1, object1);
  parse(b, prefix2, score2, object2);
  if (prefix1 != prefix2)
    return prefix1 < prefix2 ? -1 : 1;
  if (score1 != score2)
    return score1 < score2 ? -1 : 1;
  if (object1 == object2)
    return 0;
  return object1 < object2 ? -1 : 1;
}

void PrefixScoreObject::parse(const rocksdb::Slice &a, std::string &key, int64_t &score, std::string &objectID) const {
  std::string tmp = a.ToString();
  objectID.clear();
  score = 0;
  auto first = tmp.find(delim_);
  if (first == std::string::npos) {
    key = tmp;
    return;
  }
  key = tmp.substr(0, first);
  auto second = tmp.find(delim_, first + 1);
  // we allow objectID to be empty, then we can treat this table as timeserries data
  size_t len = (second == std::string::npos ? tmp.size() - first - 1 : second - first - 1);
  try {
    score = boost::lexical_cast<int64_t>(tmp.substr(first + 1, len));
  } catch (...) {
    LOG(ERROR) << "illegal input" << tmp;
    return;
  }
  if (second == std::string::npos)
    return;
  objectID = tmp.substr(second + 1);
}

}
}
}
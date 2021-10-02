#include "KeyScore.h"
#include <boost/lexical_cast.hpp>
#include <glog/logging.h>

namespace lib {
namespace engine {
namespace comparator {

KeyScore::KeyScore(char delim)
    : delim_(delim) {}
// dup code, TODO: move
bool KeyScore::validate(const std::string &input) const {
  return ComparatorUtil::validateKVValue(input, delim_) && std::isdigit(input.back());
}

int KeyScore::Compare(const rocksdb::Slice &a, const rocksdb::Slice &b) const {
  std::string prefix1, prefix2;
  int64_t score1, score2;
  parse(a, prefix1, score1);
  parse(b, prefix2, score2);
  if (prefix1 != prefix2)
    return prefix1 < prefix2 ? -1 : 1;
  if (score1 == score2)
    return 0;
  return score1 < score2 ? -1 : 1;
}

void KeyScore::parse(const rocksdb::Slice &a, std::string &key, int64_t &score) const {
  std::string tmp = a.ToString();
  score = 0;
  auto first = tmp.find(delim_);
  if (first == std::string::npos) {
    key = tmp;
    return;
  }
  key = tmp.substr(0, first);
  try {
    score = boost::lexical_cast<int64_t>(tmp.substr(first + 1));
  } catch (...) {
    LOG(ERROR) << "illegal input" << tmp;
    return;
  }
}

}
}
}
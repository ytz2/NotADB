#pragma  once
#include "Comparator.h"
#include "KeyScore.h"
#include "PrefixScoreObject.h"
namespace lib {
namespace engine {
class ComparatorFactory {
 public:
  static Comparator *createComparator(const std::string &type, char delim) {
    if (type == "KeyValue")
      return new comparator::KeyScore(delim);
    if (type == "PrefixScoredObject")
      return new comparator::PrefixScoreObject(delim);
    return nullptr;
  }
};
}
}
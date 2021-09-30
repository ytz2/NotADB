#pragma  once
#include "Comparator.h"
namespace lib {
namespace engine {
namespace comparator {

// ref: pinterest realpin
class PrefixScoreObject : public Comparator {
 public:
  PrefixScoreObject(char delim);
  virtual ~PrefixScoreObject() = default;
  // Ignore the following methods for now:
  const char *Name() const override { return "PrefixScoreFeature"; }
  void FindShortestSeparator(std::string *, const rocksdb::Slice &) const override {}
  void FindShortSuccessor(std::string *) const override {}
  int Compare(const rocksdb::Slice &a, const rocksdb::Slice &b) const override;
  bool validate(const std::string &input) const override;
  void parse(const rocksdb::Slice &a, std::string &key, int64_t &score, std::string &feature) const;
 private:
  char delim_;
};
}
}
}
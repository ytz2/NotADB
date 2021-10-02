#pragma  once
#include "Comparator.h"
namespace lib {
namespace engine {
namespace comparator {

// ref: pinterest realpin
class KeyScore : public Comparator {
 public:
  KeyScore(char delim);
  virtual ~KeyScore() = default;
  // Ignore the following methods for now:
  const char *Name() const override { return "KeyScore"; }
  void FindShortestSeparator(std::string *, const rocksdb::Slice &) const override {}
  void FindShortSuccessor(std::string *) const override {}
  int Compare(const rocksdb::Slice &a, const rocksdb::Slice &b) const override;
  bool validate(const std::string &input) const override;
  void parse(const rocksdb::Slice &a, std::string &key, int64_t &score) const;
 private:
  char delim_;
};
}
}
}
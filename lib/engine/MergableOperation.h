#pragma once
#include "message/avro/AvroMessage.h"
#include "rocksdb/db.h"
#include "rocksdb/convenience.h"
#include "rocksdb/slice.h"
#include "rocksdb/merge_operator.h"

namespace lib {
namespace engine {
typedef lib::message::avromsg::Merge MergeOperation;

class MyMergeOperator : public rocksdb::MergeOperator {
 public:
  virtual bool FullMerge(const rocksdb::Slice &key,
                         const rocksdb::Slice *existing_value,
                         const std::deque<std::string> &operand_list,
                         std::string *new_value,
                         rocksdb::Logger *logger) const override {
    return true;
  };

  virtual bool PartialMerge(const rocksdb::Slice &key,
                            const rocksdb::Slice &left_operand,
                            const rocksdb::Slice &right_operand,
                            std::string *new_value,
                            rocksdb::Logger *logger) const override {
    return true;
  }

  virtual const char *Name() const override {
    return "MyMergeOperator";
  }
};

}
}
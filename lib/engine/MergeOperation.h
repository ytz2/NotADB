#pragma once
#include "message/avro/AvroMessage.h"
#include "rocksdb/db.h"
#include "rocksdb/convenience.h"
#include "rocksdb/slice.h"
#include "rocksdb/merge_operator.h"

namespace lib {
namespace engine {
typedef lib::message::avromsg::Merge MergeOperation;

class MyMergeOperator : public rocksdb::AssociativeMergeOperator {
 public:
  virtual bool Merge(
      const rocksdb::Slice &key,
      const rocksdb::Slice *existing_value,
      const rocksdb::Slice &value,
      std::string *new_value,
      rocksdb::Logger *logger) const override;

  virtual const char *Name() const override {
    return "MyMergeOperator";
  }
};

}
}
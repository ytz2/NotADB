#include "MergeOperation.h"
#include <glog/logging.h>
#include <json/json.h>

namespace lib {
namespace engine {

namespace {

bool toJson(const rocksdb::Slice *existing_value, Json::Value& val) {
  Json::Reader reader;
  return reader.parse( existing_value->ToString(), val );
}

bool checkAndUpdteMeta(Json::Value& val, const MergeOperation &op) {
  if (op.meta.is_null())
    return true;
  auto meta = op.meta.get_MergeMeta();
  auto& jv = val["_meta"];
  if (jv["topic"] == meta.topic && jv["partition"] == meta.partition && jv["offset"] <= meta.offset)
    return false;
  jv["topic"] = meta.topic;
  jv["partition"] = meta.partition;
  jv["offset"] = meta.offset;
  return true;
}

bool mergeIncrement(const rocksdb::Slice *existing_value,
                    std::string *new_value,
                    const MergeOperation &op) {
  return true;
}

bool mergeSet(const rocksdb::Slice *existing_value,
              std::string *new_value,
              const MergeOperation &op) {
  return true;
}

bool mergeDelete(const rocksdb::Slice *existing_value,
                 std::string *new_value,
                 const MergeOperation &op) {
  if (!existing_value)
    return true;
  Json::Value val;
  if (!toJson(existing_value, val))
    return true;
  if (!checkAndUpdteMeta(val, op))
    return true;
  // todo: remove along path
  return true;
}

bool mergeAppend(const rocksdb::Slice *existing_value,
                 std::string *new_value,
                 const MergeOperation &op) {
  return true;
}
}
bool MyMergeOperator::Merge(
    const rocksdb::Slice &key,
    const rocksdb::Slice *existing_value,
    const rocksdb::Slice &value,
    std::string *new_value,
    rocksdb::Logger *logger) const {
  MergeOperation operation;
  if (!operation.FromString(value.ToString()))
    return true;
  if (operation.key != key.ToString())
    return true;

  auto type = operation.type;
  switch (type) {
    case avrogen::EMergeType::MergeIncrement: {
      return mergeIncrement(existing_value, new_value, operation);
    }
    case avrogen::EMergeType::MergeSet: {
      return mergeSet(existing_value, new_value, operation);
    }
    case avrogen::EMergeType::MergeDelete: {
      return mergeDelete(existing_value, new_value, operation);
    }
    case avrogen::EMergeType::MergeAppend: {
      return mergeAppend(existing_value, new_value, operation);
    }
  }
  return true;
}

}
}
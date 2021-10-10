#include "MergeOperation.h"
#include <glog/logging.h>
#include <json/json.h>
#include <boost/algorithm/string.hpp>
#include <deque>
// in order to record meta info to dedup the KAFKA at least once delivery
// we use json as data record, the rule is to write meta field and data field and all operations are on
// data field

namespace lib {
namespace engine {

namespace {
const static std::string DATA_FIELD = "data";
const static std::string META_FIELD = "meta";

enum class MergeCheckResult {
  NO_EXISTING_FIELD,
  NOT_JSON_BLOB,
  NO_DATA_FIELD,
  NO_META_INFO,
  DUPLICATE_MERGE,
  INVALID_OPERATION,
  SUCCESS
};

void printMergeCheckResult(MergeCheckResult res) {
  switch (res) {
    case MergeCheckResult::NO_EXISTING_FIELD:LOG(ERROR) << "value does not exist";
      break;
    case MergeCheckResult::NOT_JSON_BLOB:LOG(ERROR) << "bad data, the value is not json blob";
      break;
    case MergeCheckResult::NO_DATA_FIELD:LOG(ERROR) << "the json blob does not contain data field";
      break;
    case MergeCheckResult::NO_META_INFO:LOG(ERROR) << "the json blob does not contain meta field";
      break;
    case MergeCheckResult::DUPLICATE_MERGE:LOG(ERROR) << "the merge operation has been delivered before, ignore";
      break;
    case MergeCheckResult::INVALID_OPERATION:LOG(ERROR) << "operation input is invalid";
      break;
    case MergeCheckResult::SUCCESS:LOG(INFO) << "success";
      break;
    default:LOG(ERROR) << "unknown error code";
  }
};

std::deque<std::string> parsePath(const std::string &input) {
  std::deque<std::string> tokens;
  boost::split(tokens, input, boost::is_any_of("\t.\\,|"));
  tokens.push_front(DATA_FIELD); // default
  return tokens;
}

// existing value must have DATA_FIELD
MergeCheckResult toJson(const rocksdb::Slice *existing_value, Json::Value &val) {
  Json::Reader reader;
  if (!reader.parse(existing_value->ToString(), val))
    return MergeCheckResult::NOT_JSON_BLOB;
  if (!val.isMember(DATA_FIELD))
    return MergeCheckResult::NO_DATA_FIELD;
  return MergeCheckResult::SUCCESS;
}

// sorry, double pointer
MergeCheckResult seekToPath(const std::string &path,
                            Json::Value &root,
                            Json::Value **node,
                            std::string &next,
                            bool allowRoot = false) {
  if (path.empty())
    return MergeCheckResult::INVALID_OPERATION;
  auto tokens = parsePath(path);
  if (tokens.size() == 1 && !allowRoot)
    return MergeCheckResult::INVALID_OPERATION;
  Json::Value *nodePtr = &root;
  for (size_t i = 0; i < tokens.size() - 1; i++) {
    if (nodePtr->isMember(tokens[i]))
      nodePtr = &(*nodePtr)[tokens[i]];
    else
      return MergeCheckResult::INVALID_OPERATION;
  }
  next = tokens.back();
  if (!nodePtr->isMember(next))
    return MergeCheckResult::INVALID_OPERATION;
  *node = nodePtr;
  return MergeCheckResult::SUCCESS;
}

MergeCheckResult checkAndUpdateMeta(Json::Value &val, const MergeOperation &op) {
  if (op.meta.is_null()) // always require meta
    return MergeCheckResult::NO_META_INFO;
  auto meta = op.meta.get_MergeMeta();
  auto &jv = val[META_FIELD];
  if (jv["topic"] == meta.topic && jv["partition"] == meta.partition && jv["offset"] <= meta.offset)
    return MergeCheckResult::DUPLICATE_MERGE;
  jv["topic"] = meta.topic;
  jv["partition"] = meta.partition;
  jv["offset"] = meta.offset;
  return MergeCheckResult::SUCCESS;
}

MergeCheckResult preProcess(const rocksdb::Slice *existing_value,
                            const MergeOperation &op, Json::Value &val) {
  if (!existing_value) {
    LOG(ERROR) << "mergeDelete has not existing value";
    return MergeCheckResult::NO_EXISTING_FIELD;
  }
  auto res = toJson(existing_value, val);
  if (res != MergeCheckResult::SUCCESS) {
    return res;
  }
  res = checkAndUpdateMeta(val, op);
  if (res != MergeCheckResult::SUCCESS) {
    return res;
  }
  return MergeCheckResult::SUCCESS;
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
  Json::Value root;
  auto res = preProcess(existing_value, op, root);
  if (res != MergeCheckResult::SUCCESS) {
    printMergeCheckResult(res);
    return true;
  }
  Json::Value *parent = nullptr;
  std::string field;
  res = seekToPath(op.operation.get_MergeDelete().path, root, &parent, field);
  if (res != MergeCheckResult::SUCCESS || !parent || field.empty()) {
    return true;
  }
  parent->removeMember(field);
  Json::StreamWriterBuilder builder;
  builder["indentation"] = "";
  *new_value = Json::writeString(builder, root);
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
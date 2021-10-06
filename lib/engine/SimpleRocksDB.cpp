#include "SimpleRocksDB.h"
#include <glog/logging.h>
#include <algorithm>
#include "MergableOperation.h"

namespace lib {
namespace engine {

SimpleRocksDB::SimpleRocksDB(config::Configuration config) {
  init(config);
}

SimpleRocksDB::~SimpleRocksDB() {
  for (auto &each: columnFamilies_)
    if (each.second)
      delete each.second;
  if (rocksDB_)
    delete rocksDB_;
}

rocksdb::Options SimpleRocksDB::getDBOptions(config::Configuration config) {
  rocksdb::Options dboptions;
  dboptions.IncreaseParallelism();
  dboptions.OptimizeLevelStyleCompaction();
  dboptions.create_if_missing = false;
  dboptions.merge_operator.reset(new MyMergeOperator);
  return dboptions;
}

void SimpleRocksDB::init(config::Configuration config) {
  if (!config.get("dbName", name_)) {
    throw std::runtime_error("config does not contains dbName");
  }
  if (!config.get("dbPath", dbPath_)) {
    throw std::runtime_error("config does not contains dbPath");
  }
  rocksdb::Options dboptions = getDBOptions(config);
  // we create by ourself
  std::vector<std::string> cols;
  if (!config.get("dbColumns", cols)) {
    throw std::runtime_error("config does not contains dbColumns");
  }

  // List Column Family
  std::vector<std::string> column_family_names;
  rocksdb::Status s = rocksdb::DB::ListColumnFamilies(dboptions, dbPath_, &column_family_names);
  // db exist
  if (s.ok()) {
    std::vector<rocksdb::ColumnFamilyDescriptor> column_family_desc;
    std::vector < rocksdb::ColumnFamilyHandle * > handles;
    for (auto &column_family_name: column_family_names) {
      auto colOption = rocksdb::ColumnFamilyOptions();
      colOption.comparator = dboptions.comparator;
      column_family_desc.push_back(
          rocksdb::ColumnFamilyDescriptor(column_family_name,
                                          colOption));
    }
    s = rocksdb::DB::Open(dboptions,
                          dbPath_,
                          column_family_desc,
                          &handles,
                          &rocksDB_);
    if (s.ok()) {
      LOG(INFO) << "reopen " << dbPath_ << " success";
      for (auto &handle: handles) {
        columnFamilies_[handle->GetName()] = handle;
        LOG(INFO) << "open column family " << handle->GetName();
      }
    } else {
      throw std::runtime_error("cannot reopen db " + dbPath_ + " " + s.getState());
    }
    // check if we need to create cols per config
    for (const auto &configuredCol: cols) {
      if (std::find(std::begin(column_family_names), std::end(column_family_names), configuredCol)
          != std::end(column_family_names))
        continue;
      if (!add_column(configuredCol).ok()) {
        LOG(ERROR) << "cannot add " << configuredCol;
        throw std::runtime_error("db init exception: " + configuredCol + " cannot be created");
      } else {
        LOG(INFO) << "successfully added " << configuredCol;
      }
    }
  } else {
    dboptions.create_if_missing = true;
    LOG(INFO) << name_ << " uses comparator type: "
              << (dboptions.comparator == nullptr ? "default" : dboptions.comparator->Name());
    auto s = rocksdb::DB::Open(dboptions, dbPath_, &rocksDB_);
    if (!s.ok()) {
      throw std::runtime_error("failed to open db " + dbPath_ + " due to status code " + s.ToString());
    }

    for (const auto &each: cols) {
      if (!add_column(each).ok()) {
        throw std::runtime_error("failed to create col " + each);
      }
    }
  }

  readOptions_ = rocksdb::ReadOptions();
  writeOptions_ = rocksdb::WriteOptions();
  colDataOption_ = rocksdb::ColumnFamilyOptions();
}

rocksdb::Status SimpleRocksDB::get(const std::string &key,
                                   std::string &value,
                                   const std::string &col) {
  auto cf = getHandle(col);
  if (!cf) return rocksdb::Status::NotFound();
  return rocksDB_->Get(readOptions_, cf, key, &value);
}

std::vector<rocksdb::Status> SimpleRocksDB::multiGet(const std::vector<std::string> &keys,
                                                     std::vector<std::string> &values,
                                                     const std::string &col) {
  auto cf = getHandle(col);
  if (!cf) return std::vector<rocksdb::Status>(keys.size(), rocksdb::Status::NotFound());

  std::vector<rocksdb::Slice> skeys;
  std::vector<rocksdb::PinnableSlice> svalues;
  std::vector<rocksdb::Status> statuses;
  for (const auto &k: keys)
    skeys.emplace_back(k);
  svalues.resize(keys.size());
  statuses.resize(keys.size());
  rocksDB_->MultiGet(readOptions_, cf, keys.size(), skeys.data(), svalues.data(), statuses.data());
  for (const auto &each: statuses) {
    if (!each.ok())
      return statuses;
  }
  values.clear();
  for (const auto &each: svalues) {
    values.push_back(each.ToString());
  }
  return statuses;
}

rocksdb::Status SimpleRocksDB::write(const std::vector<std::string> &keys,
                                     const std::vector<std::string> &values,
                                     const std::string &col) {
  auto cf = getHandle(col);
  if (!cf) return rocksdb::Status::NotFound();
  rocksdb::WriteBatch batch;

  for (size_t i = 0; i < keys.size(); ++i) {
    batch.Put(cf, keys[i], values[i]);
  }
  return rocksDB_->Write(writeOptions_, &batch);
}

rocksdb::Status SimpleRocksDB::put(const std::string &key,
                                   const std::string &value,
                                   const std::string &col) {
  auto cf = getHandle(col);
  if (!cf) return rocksdb::Status::NotFound();
  return rocksDB_->Put(writeOptions_, cf, key, value);
}

rocksdb::Status SimpleRocksDB::remove(const std::string &key,
                                      const std::string &col) {
  auto cf = getHandle(col);
  if (!cf) return rocksdb::Status::NotFound();
  return rocksDB_->Delete(writeOptions_, cf, key);
}

rocksdb::Status SimpleRocksDB::remove_range(const std::string &col,
                                            const std::string &begin,
                                            const std::string &end) {
  auto cf = getHandle(col);
  if (!cf) return rocksdb::Status::NotFound();
  rocksdb::Slice b(begin), e(end);
  return rocksDB_->DeleteRange(writeOptions_, cf, b, e);
}

std::shared_ptr<rocksdb::Iterator> SimpleRocksDB::new_iterator(const std::string &col) {
  std::shared_ptr<rocksdb::Iterator> res = nullptr;
  auto cf = getHandle(col);
  if (!cf) return res;
  res.reset(rocksDB_->NewIterator(readOptions_, cf));
  return res;
}

rocksdb::Status SimpleRocksDB::add_column(const std::string &col) {
  if (getHandle(col)) return rocksdb::Status::NotFound();
  rocksdb::ColumnFamilyHandle *handler = nullptr;
  auto s = rocksDB_->CreateColumnFamily(colDataOption_, col, &handler);
  if (s.ok()) {
    columnFamilies_[col] = handler;
    LOG(INFO) << col << " has been created";
  } else {
    LOG(INFO) << col << " failed to be created";
    return rocksdb::Status::Corruption();
  }
  columnFamilies_[col] = handler;
  return rocksdb::Status::OK();
}

rocksdb::Status SimpleRocksDB::delete_column(const std::string &col) {
  auto cf = getHandle(col);
  if (!cf) return rocksdb::Status::NotFound();

  auto res = rocksDB_->DropColumnFamily(cf);
  if (!res.ok()) {
    LOG(ERROR) << col << ": " << res.ToString();
    return rocksdb::Status::Corruption();;
  }
  res = rocksDB_->DestroyColumnFamilyHandle(cf);
  if (!res.ok()) {
    LOG(ERROR) << col << ": " << res.ToString();
    return rocksdb::Status::Corruption();;
  }
  columnFamilies_.erase(col);
  return rocksdb::Status::OK();
}

rocksdb::Status SimpleRocksDB::merge(const std::string &key,
                                     const std::string &col,
                                     const interface::iSerializablePtr val
) {
  auto cf = getHandle(col);
  if (!cf) return rocksdb::Status::NotFound();
  // here we strictly require the ptr to be Merge
  const auto msg = std::dynamic_pointer_cast<MergeOperation>(val);
  if (!msg) {
    LOG(ERROR) << "merge operation for simple mode only accept Merge type";
    return rocksdb::Status::Corruption();
  }
  std::string mergeVal;
  if (!val->ToString(mergeVal)) {
    LOG(ERROR) << "cannot serialize merge operation";
    return rocksdb::Status::Corruption();
  }
  return rocksDB_->Merge(writeOptions_, cf, key, mergeVal);
}

}
}
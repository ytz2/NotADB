#pragma once
#include <string>
#include <map>
#include <vector>
#include "rocksdb/db.h"
#include "rocksdb/convenience.h"
#include "rocksdb/slice.h"
#include "rocksdb/cache.h"
#include "rocksdb/listener.h"
#include "rocksdb/options.h"
#include "rocksdb/status.h"
#include "rocksdb/slice_transform.h"
#include "rocksdb/utilities/transaction.h"
#include "rocksdb/utilities/transaction_db.h"
namespace lib {
namespace engine {

class MyRocksDB {
 public:

  MyRocksDB();
  virtual ~MyRocksDB() {
    // leave it empty on purpose, never delete db
  }

  rocksdb::Status write(const rocksdb::WriteOptions &options, rocksdb::WriteBatch *updates) {
    return rocksDB_->Write(options, updates);
  }

  rocksdb::Status write(const rocksdb::WriteOptions &options,
                        const std::vector<std::string> &keys,
                        const std::string &col,
                        const std::vector<std::string> &values) {
    auto cf = getHandle(col);
    if (!cf) return rocksdb::Status::NotFound();
    rocksdb::WriteBatch batch;

    for (size_t i = 0; i < keys.size(); ++i) {
      batch.Put(cf, keys[i], values[i]);
    }
    return rocksDB_->Write(options, &batch);
  }

  rocksdb::Status get(const rocksdb::ReadOptions &options,
                      const rocksdb::Slice &key,
                      const std::string &col,
                      std::string *value
  ) {
    auto cf = getHandle(col);
    if (!cf) return rocksdb::Status::NotFound();
    return rocksDB_->Get(options, cf, key, value);
  }

  rocksdb::Status put(const rocksdb::WriteOptions &options,
                      const rocksdb::Slice &key,
                      const std::string &col,
                      const rocksdb::Slice &value) {
    auto cf = getHandle(col);
    if (!cf) return rocksdb::Status::NotFound();
    return rocksDB_->Put(options, cf, key, value);
  }

  rocksdb::Transaction *begin_transaction(
      const rocksdb::WriteOptions &options,
      const rocksdb::TransactionOptions &txnOptions) {
    return rocksDB_->BeginTransaction(options, txnOptions);
  }

  rocksdb::Status compact_range(const rocksdb::CompactRangeOptions &options,
                                const std::string &col,
                                const rocksdb::Slice *begin,
                                const rocksdb::Slice *end) {
    auto cf = getHandle(col);
    if (!cf) return rocksdb::Status::NotFound();
    return rocksDB_->CompactRange(options, cf, begin, end);
  }

  rocksdb::Status flush(const rocksdb::FlushOptions &options,
                        const std::string &col) {
    auto cf = getHandle(col);
    if (!cf) return rocksdb::Status::NotFound();
    return rocksDB_->Flush(options, cf);
  }

  rocksdb::Status remove(const rocksdb::WriteOptions &options,
                         const rocksdb::Slice &key,
                         const std::string &col) {
    auto cf = getHandle(col);
    if (!cf) return rocksdb::Status::NotFound();
    return rocksDB_->Delete(options, cf, key);
  }

  rocksdb::Status remove_range(const rocksdb::WriteOptions &options,
                               const std::string &col,
                               const rocksdb::Slice &begin,
                               const rocksdb::Slice &end,
                               bool deleteFileInRange
  );

  rocksdb::Iterator *new_iterator(const rocksdb::ReadOptions &options,
                                  const std::string &col) {
    auto cf = getHandle(col);
    if (!cf) return nullptr;
    return rocksDB_->NewIterator(options, cf);
  }

  rocksdb::Iterator *new_iterator(const rocksdb::ReadOptions &options, const std::string cf) {
    if (columnFamilies_.count(cf) == 0) {
      return nullptr;
    }
    return rocksDB_->NewIterator(options, columnFamilies_[cf]);
  }

  rocksdb::TransactionDB *get_db() {
    return rocksDB_;
  }

  bool create_column_family(std::string col);
  bool delete_column_family(std::string col);

  rocksdb::Options get_options(const std::string &col) {
    auto cf = getHandle(col);
    if (!cf)
      throw std::runtime_error(col + " is not available");
    return rocksDB_->GetOptions(cf);
  }

  rocksdb::DBOptions get_db_options() {
    return rocksDB_->GetDBOptions();
  }

  const rocksdb::Snapshot *get_snapshot() {
    return rocksDB_->GetSnapshot();
  }
  void relase_snapshot(const rocksdb::Snapshot *snapshot) {
    rocksDB_->ReleaseSnapshot(snapshot);
  }
  void close() {
    if (rocksDB_)
      delete rocksDB_;
  }

 private:
  rocksdb::ColumnFamilyHandle *getHandle(const std::string &col) {
    if (!columnFamilies_.count(col)) return nullptr;
    return columnFamilies_[col];
  }

 private:
  std::string dbPath_;
  rocksdb::TransactionDB *rocksDB_;
  std::map<std::string, rocksdb::ColumnFamilyHandle *> columnFamilies_;
  rocksdb::ColumnFamilyOptions colDataOption_; //unified option
};

}
}
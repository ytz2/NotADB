#include "MyRocksDB.h"
#include <glog/logging.h>

namespace lib {
namespace engine {

MyRocksDB::MyRocksDB() {

}

rocksdb::Status MyRocksDB::remove_range(const rocksdb::WriteOptions &options,
                                        const std::string &col,
                                        const rocksdb::Slice &begin,
                                        const rocksdb::Slice &end,
                                        bool deleteFileInRange
) {
  auto cf = getHandle(col);
  if (!cf) return rocksdb::Status::NotFound();

  if (deleteFileInRange) {
    auto s = rocksdb::DeleteFilesInRange(rocksDB_, cf, &begin, &end, false);
    if (!s.ok()) {
      return s;
    }
  }
  return rocksDB_->DeleteRange(options, cf, begin, end);
}

bool MyRocksDB::create_column_family(std::string col) {
  if (getHandle(col)) return false;
  rocksdb::ColumnFamilyHandle *handler = nullptr;
  auto s = rocksDB_->CreateColumnFamily(colDataOption_, col, &handler);
  if (s.ok()) {
    columnFamilies_[col] = handler;
    LOG(INFO) << col << " has been created";
  } else {
    LOG(INFO) << col << " failed to be created";
    return false;
  }
  columnFamilies_[col] = handler;
  return true;
}

bool MyRocksDB::delete_column_family(std::string col) {
  auto cf = getHandle(col);
  if (!cf) return false;

  auto res = rocksDB_->DropColumnFamily(cf);
  if (!res.ok()) {
    LOG(ERROR) << col << ": " << res.ToString();
    return false;
  }
  res = rocksDB_->DestroyColumnFamilyHandle(cf);
  if (!res.ok()) {
    LOG(ERROR) << col << ": " << res.ToString();
    return false;
  }
  columnFamilies_.erase(col);
}
}
}
#pragma once
#include <vector>
#include <memory>
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

/*
 * interface for creating a RocksDB / other engine backed 'built-in' DB
 */

namespace interface {

// customize the usage of RocksDB
class iRocksDB {
 public:
  iRocksDB() = default;
  virtual ~iRocksDB()  {};
  virtual rocksdb::Status get(const std::string &key,
                              std::string &value,
                              const std::string &col
  ) = 0;

  virtual rocksdb::Status multiGet(const std::vector<std::string> &key,
                                   std::vector<std::string> &value,
                                   const std::string &col
  ) = 0;

  virtual rocksdb::Status write(rocksdb::WriteBatch *updates) = 0;

  virtual rocksdb::Status write(const std::vector<std::string> &keys,
                                const std::vector<std::string> &values,
                                const std::string &col
  ) = 0;

  virtual rocksdb::Status put(const std::string &key,
                              const std::string &value,
                              const std::string &col
  ) = 0;

  virtual rocksdb::Status remove(const std::string &&key,
                                 const std::string &col
  ) = 0;

  virtual rocksdb::Status remove_range(const std::string &col,
                                       const std::string &begin,
                                       const std::string &end,
                                       bool deleteFileInRange
  ) = 0;

  virtual std::shared_ptr<rocksdb::Iterator> new_iterator(const std::string& col)
  = 0;

  virtual rocksdb::Status add_column(const std::string& col) = 0;
  virtual rocksdb::Status delete_column(const std::string& col) = 0;
};

typedef std::shared_ptr<iRocksDB> iRocksDBPtr;
}
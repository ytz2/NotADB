#pragma once
#include "interface/db.h"
#include "lib/config/Configuration.h"
#include <map>
#include <vector>
#include <string>

namespace lib {
namespace engine {

class SimpleRocksDB : public interface::iRocksDB {
 public:
  explicit SimpleRocksDB(config::Configuration config);

  SimpleRocksDB() = delete;

  virtual ~SimpleRocksDB();

  virtual rocksdb::Status get(const std::string &key,
                              std::string &value,
                              const std::string &col
  ) override;

  virtual std::vector<rocksdb::Status> multiGet(const std::vector<std::string> &keys,
                                   std::vector<std::string> &values,
                                   const std::string &col
  ) override;

  virtual rocksdb::Status write(const std::vector<std::string> &keys,
                                const std::vector<std::string> &values,
                                const std::string &col
  ) override;

  virtual rocksdb::Status put(const std::string &key,
                              const std::string &value,
                              const std::string &col
  ) override;

  virtual rocksdb::Status remove(const std::string &key,
                                 const std::string &col
  ) override;

  virtual rocksdb::Status remove_range(const std::string &col,
                                       const std::string &begin,
                                       const std::string &end
  ) override;

  virtual std::shared_ptr<rocksdb::Iterator> new_iterator(const std::string &col) override;

  virtual rocksdb::Status add_column(const std::string &col) override;
  virtual rocksdb::Status delete_column(const std::string &col) override;

 protected:
  virtual void init(config::Configuration config);
 private:
  rocksdb::ColumnFamilyHandle *getHandle(const std::string &col) {
    if (!columnFamilies_.count(col)) return nullptr;
    return columnFamilies_[col];
  }
 protected:
  std::string dbPath_;
  rocksdb::DB *rocksDB_;
  std::map<std::string, rocksdb::ColumnFamilyHandle *> columnFamilies_;
  rocksdb::ReadOptions readOptions_;
  rocksdb::WriteOptions writeOptions_;
  rocksdb::ColumnFamilyOptions colDataOption_; //unified option
};

}
}


/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef AVRO_H_3934830805__H_
#define AVRO_H_3934830805__H_

#include <sstream>
#include <any>
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace avrogen {
struct KeyValue {
  std::string key;
  std::string value;
  KeyValue() :
      key(std::string()),
      value(std::string()) {}
};

struct kafkaMeta {
  std::string topic;
  int32_t partition;
  int64_t offset;
  kafkaMeta() :
      topic(std::string()),
      partition(int32_t()),
      offset(int64_t()) {}
};

struct PutOne {
  std::string key;
  std::string value;
  std::string column;
  kafkaMeta meta;
  PutOne() :
      key(std::string()),
      value(std::string()),
      column(std::string()),
      meta(kafkaMeta()) {}
};

struct WriteMany {
  std::vector<KeyValue> keyValues;
  std::string column;
  kafkaMeta meta;
  WriteMany() :
      keyValues(std::vector<KeyValue>()),
      column(std::string()),
      meta(kafkaMeta()) {}
};

struct RemoveOne {
  std::string key;
  std::string column;
  kafkaMeta meta;
  RemoveOne() :
      key(std::string()),
      column(std::string()),
      meta(kafkaMeta()) {}
};

struct RemoveRange {
  std::string column;
  std::string begin;
  std::string end;
  bool deleteFileInRange;
  kafkaMeta meta;
  RemoveRange() :
      column(std::string()),
      begin(std::string()),
      end(std::string()),
      deleteFileInRange(bool()),
      meta(kafkaMeta()) {}
};

struct schema_json_Union__0__ {
 private:
  size_t idx_;
  std::any value_;
 public:
  size_t idx() const { return idx_; }
  KeyValue get_KeyValue() const;
  void set_KeyValue(const KeyValue &v);
  kafkaMeta get_kafkaMeta() const;
  void set_kafkaMeta(const kafkaMeta &v);
  PutOne get_PutOne() const;
  void set_PutOne(const PutOne &v);
  WriteMany get_WriteMany() const;
  void set_WriteMany(const WriteMany &v);
  RemoveOne get_RemoveOne() const;
  void set_RemoveOne(const RemoveOne &v);
  RemoveRange get_RemoveRange() const;
  void set_RemoveRange(const RemoveRange &v);
  schema_json_Union__0__();
};

inline
KeyValue schema_json_Union__0__::get_KeyValue() const {
  if (idx_ != 0) {
    throw avro::Exception("Invalid type for union");
  }
  return std::any_cast<KeyValue>(value_);
}

inline
void schema_json_Union__0__::set_KeyValue(const KeyValue &v) {
  idx_ = 0;
  value_ = v;
}

inline
kafkaMeta schema_json_Union__0__::get_kafkaMeta() const {
  if (idx_ != 1) {
    throw avro::Exception("Invalid type for union");
  }
  return std::any_cast<kafkaMeta>(value_);
}

inline
void schema_json_Union__0__::set_kafkaMeta(const kafkaMeta &v) {
  idx_ = 1;
  value_ = v;
}

inline
PutOne schema_json_Union__0__::get_PutOne() const {
  if (idx_ != 2) {
    throw avro::Exception("Invalid type for union");
  }
  return std::any_cast<PutOne>(value_);
}

inline
void schema_json_Union__0__::set_PutOne(const PutOne &v) {
  idx_ = 2;
  value_ = v;
}

inline
WriteMany schema_json_Union__0__::get_WriteMany() const {
  if (idx_ != 3) {
    throw avro::Exception("Invalid type for union");
  }
  return std::any_cast<WriteMany>(value_);
}

inline
void schema_json_Union__0__::set_WriteMany(const WriteMany &v) {
  idx_ = 3;
  value_ = v;
}

inline
RemoveOne schema_json_Union__0__::get_RemoveOne() const {
  if (idx_ != 4) {
    throw avro::Exception("Invalid type for union");
  }
  return std::any_cast<RemoveOne>(value_);
}

inline
void schema_json_Union__0__::set_RemoveOne(const RemoveOne &v) {
  idx_ = 4;
  value_ = v;
}

inline
RemoveRange schema_json_Union__0__::get_RemoveRange() const {
  if (idx_ != 5) {
    throw avro::Exception("Invalid type for union");
  }
  return std::any_cast<RemoveRange>(value_);
}

inline
void schema_json_Union__0__::set_RemoveRange(const RemoveRange &v) {
  idx_ = 5;
  value_ = v;
}

inline schema_json_Union__0__::schema_json_Union__0__() : idx_(0), value_(KeyValue()) {}
}
namespace avro {
template<>
struct codec_traits<avrogen::KeyValue> {
  static void encode(Encoder &e, const avrogen::KeyValue &v) {
    avro::encode(e, v.key);
    avro::encode(e, v.value);
  }
  static void decode(Decoder &d, avrogen::KeyValue &v) {
    if (avro::ResolvingDecoder *rd =
        dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin();
           it != fo.end(); ++it) {
        switch (*it) {
          case 0:avro::decode(d, v.key);
            break;
          case 1:avro::decode(d, v.value);
            break;
          default:break;
        }
      }
    } else {
      avro::decode(d, v.key);
      avro::decode(d, v.value);
    }
  }
};

template<>
struct codec_traits<avrogen::kafkaMeta> {
  static void encode(Encoder &e, const avrogen::kafkaMeta &v) {
    avro::encode(e, v.topic);
    avro::encode(e, v.partition);
    avro::encode(e, v.offset);
  }
  static void decode(Decoder &d, avrogen::kafkaMeta &v) {
    if (avro::ResolvingDecoder *rd =
        dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin();
           it != fo.end(); ++it) {
        switch (*it) {
          case 0:avro::decode(d, v.topic);
            break;
          case 1:avro::decode(d, v.partition);
            break;
          case 2:avro::decode(d, v.offset);
            break;
          default:break;
        }
      }
    } else {
      avro::decode(d, v.topic);
      avro::decode(d, v.partition);
      avro::decode(d, v.offset);
    }
  }
};

template<>
struct codec_traits<avrogen::PutOne> {
  static void encode(Encoder &e, const avrogen::PutOne &v) {
    avro::encode(e, v.key);
    avro::encode(e, v.value);
    avro::encode(e, v.column);
    avro::encode(e, v.meta);
  }
  static void decode(Decoder &d, avrogen::PutOne &v) {
    if (avro::ResolvingDecoder *rd =
        dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin();
           it != fo.end(); ++it) {
        switch (*it) {
          case 0:avro::decode(d, v.key);
            break;
          case 1:avro::decode(d, v.value);
            break;
          case 2:avro::decode(d, v.column);
            break;
          case 3:avro::decode(d, v.meta);
            break;
          default:break;
        }
      }
    } else {
      avro::decode(d, v.key);
      avro::decode(d, v.value);
      avro::decode(d, v.column);
      avro::decode(d, v.meta);
    }
  }
};

template<>
struct codec_traits<avrogen::WriteMany> {
  static void encode(Encoder &e, const avrogen::WriteMany &v) {
    avro::encode(e, v.keyValues);
    avro::encode(e, v.column);
    avro::encode(e, v.meta);
  }
  static void decode(Decoder &d, avrogen::WriteMany &v) {
    if (avro::ResolvingDecoder *rd =
        dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin();
           it != fo.end(); ++it) {
        switch (*it) {
          case 0:avro::decode(d, v.keyValues);
            break;
          case 1:avro::decode(d, v.column);
            break;
          case 2:avro::decode(d, v.meta);
            break;
          default:break;
        }
      }
    } else {
      avro::decode(d, v.keyValues);
      avro::decode(d, v.column);
      avro::decode(d, v.meta);
    }
  }
};

template<>
struct codec_traits<avrogen::RemoveOne> {
  static void encode(Encoder &e, const avrogen::RemoveOne &v) {
    avro::encode(e, v.key);
    avro::encode(e, v.column);
    avro::encode(e, v.meta);
  }
  static void decode(Decoder &d, avrogen::RemoveOne &v) {
    if (avro::ResolvingDecoder *rd =
        dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin();
           it != fo.end(); ++it) {
        switch (*it) {
          case 0:avro::decode(d, v.key);
            break;
          case 1:avro::decode(d, v.column);
            break;
          case 2:avro::decode(d, v.meta);
            break;
          default:break;
        }
      }
    } else {
      avro::decode(d, v.key);
      avro::decode(d, v.column);
      avro::decode(d, v.meta);
    }
  }
};

template<>
struct codec_traits<avrogen::RemoveRange> {
  static void encode(Encoder &e, const avrogen::RemoveRange &v) {
    avro::encode(e, v.column);
    avro::encode(e, v.begin);
    avro::encode(e, v.end);
    avro::encode(e, v.deleteFileInRange);
    avro::encode(e, v.meta);
  }
  static void decode(Decoder &d, avrogen::RemoveRange &v) {
    if (avro::ResolvingDecoder *rd =
        dynamic_cast<avro::ResolvingDecoder *>(&d)) {
      const std::vector<size_t> fo = rd->fieldOrder();
      for (std::vector<size_t>::const_iterator it = fo.begin();
           it != fo.end(); ++it) {
        switch (*it) {
          case 0:avro::decode(d, v.column);
            break;
          case 1:avro::decode(d, v.begin);
            break;
          case 2:avro::decode(d, v.end);
            break;
          case 3:avro::decode(d, v.deleteFileInRange);
            break;
          case 4:avro::decode(d, v.meta);
            break;
          default:break;
        }
      }
    } else {
      avro::decode(d, v.column);
      avro::decode(d, v.begin);
      avro::decode(d, v.end);
      avro::decode(d, v.deleteFileInRange);
      avro::decode(d, v.meta);
    }
  }
};

template<>
struct codec_traits<avrogen::schema_json_Union__0__> {
  static void encode(Encoder &e, avrogen::schema_json_Union__0__ v) {
    e.encodeUnionIndex(v.idx());
    switch (v.idx()) {
      case 0:avro::encode(e, v.get_KeyValue());
        break;
      case 1:avro::encode(e, v.get_kafkaMeta());
        break;
      case 2:avro::encode(e, v.get_PutOne());
        break;
      case 3:avro::encode(e, v.get_WriteMany());
        break;
      case 4:avro::encode(e, v.get_RemoveOne());
        break;
      case 5:avro::encode(e, v.get_RemoveRange());
        break;
    }
  }
  static void decode(Decoder &d, avrogen::schema_json_Union__0__ &v) {
    size_t n = d.decodeUnionIndex();
    if (n >= 6) { throw avro::Exception("Union index too big"); }
    switch (n) {
      case 0: {
        avrogen::KeyValue vv;
        avro::decode(d, vv);
        v.set_KeyValue(vv);
      }
        break;
      case 1: {
        avrogen::kafkaMeta vv;
        avro::decode(d, vv);
        v.set_kafkaMeta(vv);
      }
        break;
      case 2: {
        avrogen::PutOne vv;
        avro::decode(d, vv);
        v.set_PutOne(vv);
      }
        break;
      case 3: {
        avrogen::WriteMany vv;
        avro::decode(d, vv);
        v.set_WriteMany(vv);
      }
        break;
      case 4: {
        avrogen::RemoveOne vv;
        avro::decode(d, vv);
        v.set_RemoveOne(vv);
      }
        break;
      case 5: {
        avrogen::RemoveRange vv;
        avro::decode(d, vv);
        v.set_RemoveRange(vv);
      }
        break;
    }
  }
};

}
#endif

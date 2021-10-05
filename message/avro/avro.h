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
        value(std::string())
        { }
};

struct PutOne {
    std::string key;
    std::string value;
    std::string column;
    PutOne() :
        key(std::string()),
        value(std::string()),
        column(std::string())
        { }
};

struct WriteMany {
    std::vector<KeyValue > keyValues;
    std::string column;
    WriteMany() :
        keyValues(std::vector<KeyValue >()),
        column(std::string())
        { }
};

struct RemoveOne {
    std::string key;
    std::string column;
    RemoveOne() :
        key(std::string()),
        column(std::string())
        { }
};

struct RemoveRange {
    std::string column;
    std::string begin;
    std::string end;
    bool deleteFileInRange;
    RemoveRange() :
        column(std::string()),
        begin(std::string()),
        end(std::string()),
        deleteFileInRange(bool())
        { }
};

struct MergeMeta {
    std::string topic;
    int32_t partition;
    int64_t offset;
    MergeMeta() :
        topic(std::string()),
        partition(int32_t()),
        offset(int64_t())
        { }
};

struct MergeIncrement {
    int32_t value;
    std::string path;
    MergeIncrement() :
        value(int32_t()),
        path(std::string())
        { }
};

struct MergeSet {
    std::string value;
    std::string path;
    MergeSet() :
        value(std::string()),
        path(std::string())
        { }
};

struct MergeDelete {
    std::string path;
    MergeDelete() :
        path(std::string())
        { }
};

struct MergeAppend {
    std::string value;
    std::string path;
    MergeAppend() :
        value(std::string()),
        path(std::string())
        { }
};

enum class EMergeType: unsigned {
    MergeIncrement,
    MergeSet,
    MergeDelete,
    MergeAppend,
};

struct schema_json_Union__0__ {
private:
    size_t idx_;
    std::any value_;
public:
    size_t idx() const { return idx_; }
    MergeIncrement get_MergeIncrement() const;
    void set_MergeIncrement(const MergeIncrement& v);
    MergeSet get_MergeSet() const;
    void set_MergeSet(const MergeSet& v);
    MergeDelete get_MergeDelete() const;
    void set_MergeDelete(const MergeDelete& v);
    MergeAppend get_MergeAppend() const;
    void set_MergeAppend(const MergeAppend& v);
    schema_json_Union__0__();
};

struct schema_json_Union__1__ {
private:
    size_t idx_;
    std::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = std::any();
    }
    MergeMeta get_MergeMeta() const;
    void set_MergeMeta(const MergeMeta& v);
    schema_json_Union__1__();
};

struct Merge {
    typedef schema_json_Union__0__ operation_t;
    typedef schema_json_Union__1__ meta_t;
    std::string key;
    std::string column;
    EMergeType type;
    operation_t operation;
    meta_t meta;
    Merge() :
        key(std::string()),
        column(std::string()),
        type(EMergeType()),
        operation(operation_t()),
        meta(meta_t())
        { }
};

struct schema_json_Union__2__ {
private:
    size_t idx_;
    std::any value_;
public:
    size_t idx() const { return idx_; }
    KeyValue get_KeyValue() const;
    void set_KeyValue(const KeyValue& v);
    PutOne get_PutOne() const;
    void set_PutOne(const PutOne& v);
    WriteMany get_WriteMany() const;
    void set_WriteMany(const WriteMany& v);
    RemoveOne get_RemoveOne() const;
    void set_RemoveOne(const RemoveOne& v);
    RemoveRange get_RemoveRange() const;
    void set_RemoveRange(const RemoveRange& v);
    MergeMeta get_MergeMeta() const;
    void set_MergeMeta(const MergeMeta& v);
    MergeIncrement get_MergeIncrement() const;
    void set_MergeIncrement(const MergeIncrement& v);
    MergeSet get_MergeSet() const;
    void set_MergeSet(const MergeSet& v);
    MergeDelete get_MergeDelete() const;
    void set_MergeDelete(const MergeDelete& v);
    MergeAppend get_MergeAppend() const;
    void set_MergeAppend(const MergeAppend& v);
    Merge get_Merge() const;
    void set_Merge(const Merge& v);
    schema_json_Union__2__();
};

inline
MergeIncrement schema_json_Union__0__::get_MergeIncrement() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeIncrement >(value_);
}

inline
void schema_json_Union__0__::set_MergeIncrement(const MergeIncrement& v) {
    idx_ = 0;
    value_ = v;
}

inline
MergeSet schema_json_Union__0__::get_MergeSet() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeSet >(value_);
}

inline
void schema_json_Union__0__::set_MergeSet(const MergeSet& v) {
    idx_ = 1;
    value_ = v;
}

inline
MergeDelete schema_json_Union__0__::get_MergeDelete() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeDelete >(value_);
}

inline
void schema_json_Union__0__::set_MergeDelete(const MergeDelete& v) {
    idx_ = 2;
    value_ = v;
}

inline
MergeAppend schema_json_Union__0__::get_MergeAppend() const {
    if (idx_ != 3) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeAppend >(value_);
}

inline
void schema_json_Union__0__::set_MergeAppend(const MergeAppend& v) {
    idx_ = 3;
    value_ = v;
}

inline
MergeMeta schema_json_Union__1__::get_MergeMeta() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeMeta >(value_);
}

inline
void schema_json_Union__1__::set_MergeMeta(const MergeMeta& v) {
    idx_ = 1;
    value_ = v;
}

inline
KeyValue schema_json_Union__2__::get_KeyValue() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<KeyValue >(value_);
}

inline
void schema_json_Union__2__::set_KeyValue(const KeyValue& v) {
    idx_ = 0;
    value_ = v;
}

inline
PutOne schema_json_Union__2__::get_PutOne() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<PutOne >(value_);
}

inline
void schema_json_Union__2__::set_PutOne(const PutOne& v) {
    idx_ = 1;
    value_ = v;
}

inline
WriteMany schema_json_Union__2__::get_WriteMany() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<WriteMany >(value_);
}

inline
void schema_json_Union__2__::set_WriteMany(const WriteMany& v) {
    idx_ = 2;
    value_ = v;
}

inline
RemoveOne schema_json_Union__2__::get_RemoveOne() const {
    if (idx_ != 3) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<RemoveOne >(value_);
}

inline
void schema_json_Union__2__::set_RemoveOne(const RemoveOne& v) {
    idx_ = 3;
    value_ = v;
}

inline
RemoveRange schema_json_Union__2__::get_RemoveRange() const {
    if (idx_ != 4) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<RemoveRange >(value_);
}

inline
void schema_json_Union__2__::set_RemoveRange(const RemoveRange& v) {
    idx_ = 4;
    value_ = v;
}

inline
MergeMeta schema_json_Union__2__::get_MergeMeta() const {
    if (idx_ != 5) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeMeta >(value_);
}

inline
void schema_json_Union__2__::set_MergeMeta(const MergeMeta& v) {
    idx_ = 5;
    value_ = v;
}

inline
MergeIncrement schema_json_Union__2__::get_MergeIncrement() const {
    if (idx_ != 6) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeIncrement >(value_);
}

inline
void schema_json_Union__2__::set_MergeIncrement(const MergeIncrement& v) {
    idx_ = 6;
    value_ = v;
}

inline
MergeSet schema_json_Union__2__::get_MergeSet() const {
    if (idx_ != 7) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeSet >(value_);
}

inline
void schema_json_Union__2__::set_MergeSet(const MergeSet& v) {
    idx_ = 7;
    value_ = v;
}

inline
MergeDelete schema_json_Union__2__::get_MergeDelete() const {
    if (idx_ != 8) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeDelete >(value_);
}

inline
void schema_json_Union__2__::set_MergeDelete(const MergeDelete& v) {
    idx_ = 8;
    value_ = v;
}

inline
MergeAppend schema_json_Union__2__::get_MergeAppend() const {
    if (idx_ != 9) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeAppend >(value_);
}

inline
void schema_json_Union__2__::set_MergeAppend(const MergeAppend& v) {
    idx_ = 9;
    value_ = v;
}

inline
Merge schema_json_Union__2__::get_Merge() const {
    if (idx_ != 10) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<Merge >(value_);
}

inline
void schema_json_Union__2__::set_Merge(const Merge& v) {
    idx_ = 10;
    value_ = v;
}

inline schema_json_Union__0__::schema_json_Union__0__() : idx_(0), value_(MergeIncrement()) { }
inline schema_json_Union__1__::schema_json_Union__1__() : idx_(0) { }
inline schema_json_Union__2__::schema_json_Union__2__() : idx_(0), value_(KeyValue()) { }
}
namespace avro {
template<> struct codec_traits<avrogen::KeyValue> {
    static void encode(Encoder& e, const avrogen::KeyValue& v) {
        avro::encode(e, v.key);
        avro::encode(e, v.value);
    }
    static void decode(Decoder& d, avrogen::KeyValue& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.key);
                    break;
                case 1:
                    avro::decode(d, v.value);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.key);
            avro::decode(d, v.value);
        }
    }
};

template<> struct codec_traits<avrogen::PutOne> {
    static void encode(Encoder& e, const avrogen::PutOne& v) {
        avro::encode(e, v.key);
        avro::encode(e, v.value);
        avro::encode(e, v.column);
    }
    static void decode(Decoder& d, avrogen::PutOne& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.key);
                    break;
                case 1:
                    avro::decode(d, v.value);
                    break;
                case 2:
                    avro::decode(d, v.column);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.key);
            avro::decode(d, v.value);
            avro::decode(d, v.column);
        }
    }
};

template<> struct codec_traits<avrogen::WriteMany> {
    static void encode(Encoder& e, const avrogen::WriteMany& v) {
        avro::encode(e, v.keyValues);
        avro::encode(e, v.column);
    }
    static void decode(Decoder& d, avrogen::WriteMany& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.keyValues);
                    break;
                case 1:
                    avro::decode(d, v.column);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.keyValues);
            avro::decode(d, v.column);
        }
    }
};

template<> struct codec_traits<avrogen::RemoveOne> {
    static void encode(Encoder& e, const avrogen::RemoveOne& v) {
        avro::encode(e, v.key);
        avro::encode(e, v.column);
    }
    static void decode(Decoder& d, avrogen::RemoveOne& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.key);
                    break;
                case 1:
                    avro::decode(d, v.column);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.key);
            avro::decode(d, v.column);
        }
    }
};

template<> struct codec_traits<avrogen::RemoveRange> {
    static void encode(Encoder& e, const avrogen::RemoveRange& v) {
        avro::encode(e, v.column);
        avro::encode(e, v.begin);
        avro::encode(e, v.end);
        avro::encode(e, v.deleteFileInRange);
    }
    static void decode(Decoder& d, avrogen::RemoveRange& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.column);
                    break;
                case 1:
                    avro::decode(d, v.begin);
                    break;
                case 2:
                    avro::decode(d, v.end);
                    break;
                case 3:
                    avro::decode(d, v.deleteFileInRange);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.column);
            avro::decode(d, v.begin);
            avro::decode(d, v.end);
            avro::decode(d, v.deleteFileInRange);
        }
    }
};

template<> struct codec_traits<avrogen::MergeMeta> {
    static void encode(Encoder& e, const avrogen::MergeMeta& v) {
        avro::encode(e, v.topic);
        avro::encode(e, v.partition);
        avro::encode(e, v.offset);
    }
    static void decode(Decoder& d, avrogen::MergeMeta& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.topic);
                    break;
                case 1:
                    avro::decode(d, v.partition);
                    break;
                case 2:
                    avro::decode(d, v.offset);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.topic);
            avro::decode(d, v.partition);
            avro::decode(d, v.offset);
        }
    }
};

template<> struct codec_traits<avrogen::MergeIncrement> {
    static void encode(Encoder& e, const avrogen::MergeIncrement& v) {
        avro::encode(e, v.value);
        avro::encode(e, v.path);
    }
    static void decode(Decoder& d, avrogen::MergeIncrement& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.value);
                    break;
                case 1:
                    avro::decode(d, v.path);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.value);
            avro::decode(d, v.path);
        }
    }
};

template<> struct codec_traits<avrogen::MergeSet> {
    static void encode(Encoder& e, const avrogen::MergeSet& v) {
        avro::encode(e, v.value);
        avro::encode(e, v.path);
    }
    static void decode(Decoder& d, avrogen::MergeSet& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.value);
                    break;
                case 1:
                    avro::decode(d, v.path);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.value);
            avro::decode(d, v.path);
        }
    }
};

template<> struct codec_traits<avrogen::MergeDelete> {
    static void encode(Encoder& e, const avrogen::MergeDelete& v) {
        avro::encode(e, v.path);
    }
    static void decode(Decoder& d, avrogen::MergeDelete& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.path);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.path);
        }
    }
};

template<> struct codec_traits<avrogen::MergeAppend> {
    static void encode(Encoder& e, const avrogen::MergeAppend& v) {
        avro::encode(e, v.value);
        avro::encode(e, v.path);
    }
    static void decode(Decoder& d, avrogen::MergeAppend& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.value);
                    break;
                case 1:
                    avro::decode(d, v.path);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.value);
            avro::decode(d, v.path);
        }
    }
};

template<> struct codec_traits<avrogen::EMergeType> {
    static void encode(Encoder& e, avrogen::EMergeType v) {
        if (v > avrogen::EMergeType::MergeAppend)
        {
            std::ostringstream error;
            error << "enum value " << static_cast<unsigned>(v) << " is out of bound for avrogen::EMergeType and cannot be encoded";
            throw avro::Exception(error.str());
        }
        e.encodeEnum(static_cast<size_t>(v));
    }
    static void decode(Decoder& d, avrogen::EMergeType& v) {
        size_t index = d.decodeEnum();
        if (index > static_cast<size_t>(avrogen::EMergeType::MergeAppend))
        {
            std::ostringstream error;
            error << "enum value " << index << " is out of bound for avrogen::EMergeType and cannot be decoded";
            throw avro::Exception(error.str());
        }
        v = static_cast<avrogen::EMergeType>(index);
    }
};

template<> struct codec_traits<avrogen::schema_json_Union__0__> {
    static void encode(Encoder& e, avrogen::schema_json_Union__0__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_MergeIncrement());
            break;
        case 1:
            avro::encode(e, v.get_MergeSet());
            break;
        case 2:
            avro::encode(e, v.get_MergeDelete());
            break;
        case 3:
            avro::encode(e, v.get_MergeAppend());
            break;
        }
    }
    static void decode(Decoder& d, avrogen::schema_json_Union__0__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 4) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                avrogen::MergeIncrement vv;
                avro::decode(d, vv);
                v.set_MergeIncrement(vv);
            }
            break;
        case 1:
            {
                avrogen::MergeSet vv;
                avro::decode(d, vv);
                v.set_MergeSet(vv);
            }
            break;
        case 2:
            {
                avrogen::MergeDelete vv;
                avro::decode(d, vv);
                v.set_MergeDelete(vv);
            }
            break;
        case 3:
            {
                avrogen::MergeAppend vv;
                avro::decode(d, vv);
                v.set_MergeAppend(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<avrogen::schema_json_Union__1__> {
    static void encode(Encoder& e, avrogen::schema_json_Union__1__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_MergeMeta());
            break;
        }
    }
    static void decode(Decoder& d, avrogen::schema_json_Union__1__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                avrogen::MergeMeta vv;
                avro::decode(d, vv);
                v.set_MergeMeta(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<avrogen::Merge> {
    static void encode(Encoder& e, const avrogen::Merge& v) {
        avro::encode(e, v.key);
        avro::encode(e, v.column);
        avro::encode(e, v.type);
        avro::encode(e, v.operation);
        avro::encode(e, v.meta);
    }
    static void decode(Decoder& d, avrogen::Merge& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.key);
                    break;
                case 1:
                    avro::decode(d, v.column);
                    break;
                case 2:
                    avro::decode(d, v.type);
                    break;
                case 3:
                    avro::decode(d, v.operation);
                    break;
                case 4:
                    avro::decode(d, v.meta);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.key);
            avro::decode(d, v.column);
            avro::decode(d, v.type);
            avro::decode(d, v.operation);
            avro::decode(d, v.meta);
        }
    }
};

template<> struct codec_traits<avrogen::schema_json_Union__2__> {
    static void encode(Encoder& e, avrogen::schema_json_Union__2__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_KeyValue());
            break;
        case 1:
            avro::encode(e, v.get_PutOne());
            break;
        case 2:
            avro::encode(e, v.get_WriteMany());
            break;
        case 3:
            avro::encode(e, v.get_RemoveOne());
            break;
        case 4:
            avro::encode(e, v.get_RemoveRange());
            break;
        case 5:
            avro::encode(e, v.get_MergeMeta());
            break;
        case 6:
            avro::encode(e, v.get_MergeIncrement());
            break;
        case 7:
            avro::encode(e, v.get_MergeSet());
            break;
        case 8:
            avro::encode(e, v.get_MergeDelete());
            break;
        case 9:
            avro::encode(e, v.get_MergeAppend());
            break;
        case 10:
            avro::encode(e, v.get_Merge());
            break;
        }
    }
    static void decode(Decoder& d, avrogen::schema_json_Union__2__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 11) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                avrogen::KeyValue vv;
                avro::decode(d, vv);
                v.set_KeyValue(vv);
            }
            break;
        case 1:
            {
                avrogen::PutOne vv;
                avro::decode(d, vv);
                v.set_PutOne(vv);
            }
            break;
        case 2:
            {
                avrogen::WriteMany vv;
                avro::decode(d, vv);
                v.set_WriteMany(vv);
            }
            break;
        case 3:
            {
                avrogen::RemoveOne vv;
                avro::decode(d, vv);
                v.set_RemoveOne(vv);
            }
            break;
        case 4:
            {
                avrogen::RemoveRange vv;
                avro::decode(d, vv);
                v.set_RemoveRange(vv);
            }
            break;
        case 5:
            {
                avrogen::MergeMeta vv;
                avro::decode(d, vv);
                v.set_MergeMeta(vv);
            }
            break;
        case 6:
            {
                avrogen::MergeIncrement vv;
                avro::decode(d, vv);
                v.set_MergeIncrement(vv);
            }
            break;
        case 7:
            {
                avrogen::MergeSet vv;
                avro::decode(d, vv);
                v.set_MergeSet(vv);
            }
            break;
        case 8:
            {
                avrogen::MergeDelete vv;
                avro::decode(d, vv);
                v.set_MergeDelete(vv);
            }
            break;
        case 9:
            {
                avrogen::MergeAppend vv;
                avro::decode(d, vv);
                v.set_MergeAppend(vv);
            }
            break;
        case 10:
            {
                avrogen::Merge vv;
                avro::decode(d, vv);
                v.set_Merge(vv);
            }
            break;
        }
    }
};

}
#endif

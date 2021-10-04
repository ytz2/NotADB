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

struct MergeIncrementNum {
    int32_t value;
    std::string operand;
    MergeIncrementNum() :
        value(int32_t()),
        operand(std::string())
        { }
};

struct MergeUpdateNum {
    int32_t value;
    std::string operand;
    MergeUpdateNum() :
        value(int32_t()),
        operand(std::string())
        { }
};

struct MergeUpdateString {
    std::string value;
    std::string operand;
    MergeUpdateString() :
        value(std::string()),
        operand(std::string())
        { }
};

struct MergeDeleteKey {
    std::string key;
    std::string operand;
    MergeDeleteKey() :
        key(std::string()),
        operand(std::string())
        { }
};

struct MergeAppendString {
    std::string value;
    std::string operand;
    MergeAppendString() :
        value(std::string()),
        operand(std::string())
        { }
};

struct schema_json_Union__0__ {
private:
    size_t idx_;
    std::any value_;
public:
    size_t idx() const { return idx_; }
    MergeIncrementNum get_MergeIncrementNum() const;
    void set_MergeIncrementNum(const MergeIncrementNum& v);
    MergeUpdateNum get_MergeUpdateNum() const;
    void set_MergeUpdateNum(const MergeUpdateNum& v);
    MergeUpdateString get_MergeUpdateString() const;
    void set_MergeUpdateString(const MergeUpdateString& v);
    MergeAppendString get_MergeAppendString() const;
    void set_MergeAppendString(const MergeAppendString& v);
    MergeDeleteKey get_MergeDeleteKey() const;
    void set_MergeDeleteKey(const MergeDeleteKey& v);
    schema_json_Union__0__();
};

struct Merge {
    std::string key;
    std::string column;
    std::vector<schema_json_Union__0__ > operators;
    Merge() :
        key(std::string()),
        column(std::string()),
        operators(std::vector<schema_json_Union__0__ >())
        { }
};

struct schema_json_Union__1__ {
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
    MergeIncrementNum get_MergeIncrementNum() const;
    void set_MergeIncrementNum(const MergeIncrementNum& v);
    MergeUpdateNum get_MergeUpdateNum() const;
    void set_MergeUpdateNum(const MergeUpdateNum& v);
    MergeUpdateString get_MergeUpdateString() const;
    void set_MergeUpdateString(const MergeUpdateString& v);
    MergeDeleteKey get_MergeDeleteKey() const;
    void set_MergeDeleteKey(const MergeDeleteKey& v);
    MergeAppendString get_MergeAppendString() const;
    void set_MergeAppendString(const MergeAppendString& v);
    Merge get_Merge() const;
    void set_Merge(const Merge& v);
    schema_json_Union__1__();
};

inline
MergeIncrementNum schema_json_Union__0__::get_MergeIncrementNum() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeIncrementNum >(value_);
}

inline
void schema_json_Union__0__::set_MergeIncrementNum(const MergeIncrementNum& v) {
    idx_ = 0;
    value_ = v;
}

inline
MergeUpdateNum schema_json_Union__0__::get_MergeUpdateNum() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeUpdateNum >(value_);
}

inline
void schema_json_Union__0__::set_MergeUpdateNum(const MergeUpdateNum& v) {
    idx_ = 1;
    value_ = v;
}

inline
MergeUpdateString schema_json_Union__0__::get_MergeUpdateString() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeUpdateString >(value_);
}

inline
void schema_json_Union__0__::set_MergeUpdateString(const MergeUpdateString& v) {
    idx_ = 2;
    value_ = v;
}

inline
MergeAppendString schema_json_Union__0__::get_MergeAppendString() const {
    if (idx_ != 3) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeAppendString >(value_);
}

inline
void schema_json_Union__0__::set_MergeAppendString(const MergeAppendString& v) {
    idx_ = 3;
    value_ = v;
}

inline
MergeDeleteKey schema_json_Union__0__::get_MergeDeleteKey() const {
    if (idx_ != 4) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeDeleteKey >(value_);
}

inline
void schema_json_Union__0__::set_MergeDeleteKey(const MergeDeleteKey& v) {
    idx_ = 4;
    value_ = v;
}

inline
KeyValue schema_json_Union__1__::get_KeyValue() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<KeyValue >(value_);
}

inline
void schema_json_Union__1__::set_KeyValue(const KeyValue& v) {
    idx_ = 0;
    value_ = v;
}

inline
PutOne schema_json_Union__1__::get_PutOne() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<PutOne >(value_);
}

inline
void schema_json_Union__1__::set_PutOne(const PutOne& v) {
    idx_ = 1;
    value_ = v;
}

inline
WriteMany schema_json_Union__1__::get_WriteMany() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<WriteMany >(value_);
}

inline
void schema_json_Union__1__::set_WriteMany(const WriteMany& v) {
    idx_ = 2;
    value_ = v;
}

inline
RemoveOne schema_json_Union__1__::get_RemoveOne() const {
    if (idx_ != 3) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<RemoveOne >(value_);
}

inline
void schema_json_Union__1__::set_RemoveOne(const RemoveOne& v) {
    idx_ = 3;
    value_ = v;
}

inline
RemoveRange schema_json_Union__1__::get_RemoveRange() const {
    if (idx_ != 4) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<RemoveRange >(value_);
}

inline
void schema_json_Union__1__::set_RemoveRange(const RemoveRange& v) {
    idx_ = 4;
    value_ = v;
}

inline
MergeIncrementNum schema_json_Union__1__::get_MergeIncrementNum() const {
    if (idx_ != 5) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeIncrementNum >(value_);
}

inline
void schema_json_Union__1__::set_MergeIncrementNum(const MergeIncrementNum& v) {
    idx_ = 5;
    value_ = v;
}

inline
MergeUpdateNum schema_json_Union__1__::get_MergeUpdateNum() const {
    if (idx_ != 6) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeUpdateNum >(value_);
}

inline
void schema_json_Union__1__::set_MergeUpdateNum(const MergeUpdateNum& v) {
    idx_ = 6;
    value_ = v;
}

inline
MergeUpdateString schema_json_Union__1__::get_MergeUpdateString() const {
    if (idx_ != 7) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeUpdateString >(value_);
}

inline
void schema_json_Union__1__::set_MergeUpdateString(const MergeUpdateString& v) {
    idx_ = 7;
    value_ = v;
}

inline
MergeDeleteKey schema_json_Union__1__::get_MergeDeleteKey() const {
    if (idx_ != 8) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeDeleteKey >(value_);
}

inline
void schema_json_Union__1__::set_MergeDeleteKey(const MergeDeleteKey& v) {
    idx_ = 8;
    value_ = v;
}

inline
MergeAppendString schema_json_Union__1__::get_MergeAppendString() const {
    if (idx_ != 9) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<MergeAppendString >(value_);
}

inline
void schema_json_Union__1__::set_MergeAppendString(const MergeAppendString& v) {
    idx_ = 9;
    value_ = v;
}

inline
Merge schema_json_Union__1__::get_Merge() const {
    if (idx_ != 10) {
        throw avro::Exception("Invalid type for union");
    }
    return std::any_cast<Merge >(value_);
}

inline
void schema_json_Union__1__::set_Merge(const Merge& v) {
    idx_ = 10;
    value_ = v;
}

inline schema_json_Union__0__::schema_json_Union__0__() : idx_(0), value_(MergeIncrementNum()) { }
inline schema_json_Union__1__::schema_json_Union__1__() : idx_(0), value_(KeyValue()) { }
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

template<> struct codec_traits<avrogen::MergeIncrementNum> {
    static void encode(Encoder& e, const avrogen::MergeIncrementNum& v) {
        avro::encode(e, v.value);
        avro::encode(e, v.operand);
    }
    static void decode(Decoder& d, avrogen::MergeIncrementNum& v) {
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
                    avro::decode(d, v.operand);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.value);
            avro::decode(d, v.operand);
        }
    }
};

template<> struct codec_traits<avrogen::MergeUpdateNum> {
    static void encode(Encoder& e, const avrogen::MergeUpdateNum& v) {
        avro::encode(e, v.value);
        avro::encode(e, v.operand);
    }
    static void decode(Decoder& d, avrogen::MergeUpdateNum& v) {
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
                    avro::decode(d, v.operand);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.value);
            avro::decode(d, v.operand);
        }
    }
};

template<> struct codec_traits<avrogen::MergeUpdateString> {
    static void encode(Encoder& e, const avrogen::MergeUpdateString& v) {
        avro::encode(e, v.value);
        avro::encode(e, v.operand);
    }
    static void decode(Decoder& d, avrogen::MergeUpdateString& v) {
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
                    avro::decode(d, v.operand);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.value);
            avro::decode(d, v.operand);
        }
    }
};

template<> struct codec_traits<avrogen::MergeDeleteKey> {
    static void encode(Encoder& e, const avrogen::MergeDeleteKey& v) {
        avro::encode(e, v.key);
        avro::encode(e, v.operand);
    }
    static void decode(Decoder& d, avrogen::MergeDeleteKey& v) {
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
                    avro::decode(d, v.operand);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.key);
            avro::decode(d, v.operand);
        }
    }
};

template<> struct codec_traits<avrogen::MergeAppendString> {
    static void encode(Encoder& e, const avrogen::MergeAppendString& v) {
        avro::encode(e, v.value);
        avro::encode(e, v.operand);
    }
    static void decode(Decoder& d, avrogen::MergeAppendString& v) {
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
                    avro::decode(d, v.operand);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.value);
            avro::decode(d, v.operand);
        }
    }
};

template<> struct codec_traits<avrogen::schema_json_Union__0__> {
    static void encode(Encoder& e, avrogen::schema_json_Union__0__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_MergeIncrementNum());
            break;
        case 1:
            avro::encode(e, v.get_MergeUpdateNum());
            break;
        case 2:
            avro::encode(e, v.get_MergeUpdateString());
            break;
        case 3:
            avro::encode(e, v.get_MergeAppendString());
            break;
        case 4:
            avro::encode(e, v.get_MergeDeleteKey());
            break;
        }
    }
    static void decode(Decoder& d, avrogen::schema_json_Union__0__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 5) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                avrogen::MergeIncrementNum vv;
                avro::decode(d, vv);
                v.set_MergeIncrementNum(vv);
            }
            break;
        case 1:
            {
                avrogen::MergeUpdateNum vv;
                avro::decode(d, vv);
                v.set_MergeUpdateNum(vv);
            }
            break;
        case 2:
            {
                avrogen::MergeUpdateString vv;
                avro::decode(d, vv);
                v.set_MergeUpdateString(vv);
            }
            break;
        case 3:
            {
                avrogen::MergeAppendString vv;
                avro::decode(d, vv);
                v.set_MergeAppendString(vv);
            }
            break;
        case 4:
            {
                avrogen::MergeDeleteKey vv;
                avro::decode(d, vv);
                v.set_MergeDeleteKey(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<avrogen::Merge> {
    static void encode(Encoder& e, const avrogen::Merge& v) {
        avro::encode(e, v.key);
        avro::encode(e, v.column);
        avro::encode(e, v.operators);
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
                    avro::decode(d, v.operators);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.key);
            avro::decode(d, v.column);
            avro::decode(d, v.operators);
        }
    }
};

template<> struct codec_traits<avrogen::schema_json_Union__1__> {
    static void encode(Encoder& e, avrogen::schema_json_Union__1__ v) {
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
            avro::encode(e, v.get_MergeIncrementNum());
            break;
        case 6:
            avro::encode(e, v.get_MergeUpdateNum());
            break;
        case 7:
            avro::encode(e, v.get_MergeUpdateString());
            break;
        case 8:
            avro::encode(e, v.get_MergeDeleteKey());
            break;
        case 9:
            avro::encode(e, v.get_MergeAppendString());
            break;
        case 10:
            avro::encode(e, v.get_Merge());
            break;
        }
    }
    static void decode(Decoder& d, avrogen::schema_json_Union__1__& v) {
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
                avrogen::MergeIncrementNum vv;
                avro::decode(d, vv);
                v.set_MergeIncrementNum(vv);
            }
            break;
        case 6:
            {
                avrogen::MergeUpdateNum vv;
                avro::decode(d, vv);
                v.set_MergeUpdateNum(vv);
            }
            break;
        case 7:
            {
                avrogen::MergeUpdateString vv;
                avro::decode(d, vv);
                v.set_MergeUpdateString(vv);
            }
            break;
        case 8:
            {
                avrogen::MergeDeleteKey vv;
                avro::decode(d, vv);
                v.set_MergeDeleteKey(vv);
            }
            break;
        case 9:
            {
                avrogen::MergeAppendString vv;
                avro::decode(d, vv);
                v.set_MergeAppendString(vv);
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

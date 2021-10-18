#pragma once
#include "avro.h"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"
#include "interface/message.h"
#include <sstream>
#include <iostream>

namespace lib {
namespace message {
namespace avromsg {

class AvroMessage : public interface::IMessage {
 public:  // IMessage
  virtual const std::string GetProtocol() override { return "Avro"; };
  virtual void setMetaData(const avrogen::kafkaMeta &meta) = 0;
};

template<class avrogenType>
class AvroMessageHelper {
 protected:
  bool ToStringImp(std::string &output, const avrogenType &obj) const {
    try {
      std::ostringstream os;
      std::unique_ptr<avro::OutputStream> out = avro::ostreamOutputStream(os);
      avro::EncoderPtr e = avro::binaryEncoder();
      e->init(*out);
      avro::encode(*e, obj);
      e->flush(); // this is must have
      output = os.str();
    } catch (std::exception &e) {
      return false;
    } catch (...) {
      return false;
    }
    return true;
  }

  bool FromStringImp(const std::string &input, avrogenType &obj) {
    try {
      auto isp = avro::memoryInputStream((uint8_t *) input.c_str(), input.size());
      auto d = avro::binaryDecoder();
      d->init(*isp);
      avro::decode(*d, obj);
    } catch (std::exception &e) {
      return false;
    } catch (...) {
      return false;
    }
    return true;
  }
};
// TODO: codegen below

enum class EMessageType {
  KeyValue,
  PutOne,
  WriteMany,
  RemoveOne,
  RemoveRange,
};

class KeyValue : public avrogen::KeyValue, public AvroMessage, public AvroMessageHelper<avrogen::KeyValue> {
 public:
  virtual int GetMessageID() override { return static_cast<int>(EMessageType::KeyValue); }
  virtual const std::string GetMessageName() override { return "KeyValue"; }
  bool ToString(std::string &output) const override {
    return ToStringImp(output, *this);
  }
  bool FromString(const std::string &input) override {
    return FromStringImp(input, *this);
  }
  virtual void setMetaData(const avrogen::kafkaMeta &meta) override {
    return; // no meta
  }
};

class PutOne : public avrogen::PutOne, public AvroMessage, public AvroMessageHelper<avrogen::PutOne> {
 public:
  virtual int GetMessageID() override { return static_cast<int>(EMessageType::PutOne); }
  virtual const std::string GetMessageName() override { return "PutOne"; }
  bool ToString(std::string &output) const override {
    return ToStringImp(output, *this);
  }
  bool FromString(const std::string &input) override {
    return FromStringImp(input, *this);
  }
  virtual void setMetaData(const avrogen::kafkaMeta &meta) override {
    this->meta = meta;
  }
};

class WriteMany : public avrogen::WriteMany, public AvroMessage, public AvroMessageHelper<avrogen::WriteMany> {
 public:
  virtual int GetMessageID() override { return static_cast<int>(EMessageType::WriteMany); }
  virtual const std::string GetMessageName() override { return "WriteMany"; }
  bool ToString(std::string &output) const override {
    return ToStringImp(output, *this);
  }
  bool FromString(const std::string &input) override {
    return FromStringImp(input, *this);
  }
  virtual void setMetaData(const avrogen::kafkaMeta &meta) override {
    this->meta = meta;
  }
};

class RemoveOne : public avrogen::RemoveOne, public AvroMessage, public AvroMessageHelper<avrogen::RemoveOne> {
 public:
  virtual int GetMessageID() override { return static_cast<int>(EMessageType::RemoveOne); }
  virtual const std::string GetMessageName() override { return "RemoveOne"; }
  bool ToString(std::string &output) const override {
    return ToStringImp(output, *this);
  }
  bool FromString(const std::string &input) override {
    return FromStringImp(input, *this);
  }
  virtual void setMetaData(const avrogen::kafkaMeta &meta) override {
    this->meta = meta;
  }
};

class RemoveRange : public avrogen::RemoveRange, public AvroMessage, public AvroMessageHelper<avrogen::RemoveRange> {
 public:
  virtual int GetMessageID() override { return static_cast<int>(EMessageType::RemoveRange); }
  virtual const std::string GetMessageName() override { return "RemoveRange"; }
  bool ToString(std::string &output) const override {
    return ToStringImp(output, *this);
  }
  bool FromString(const std::string &input) override {
    return FromStringImp(input, *this);
  }
  virtual void setMetaData(const avrogen::kafkaMeta &meta) override {
    this->meta = meta;
  }
};
}
}
}

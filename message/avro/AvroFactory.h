#pragma once
#include <memory>
#include <string>

#include "interface/message.h"
#include "message/avro/AvroMessage.h"
#include <map>

namespace lib {
namespace message {
namespace avromsg {

class AvroFactory : public interface::IMessageFactory {
 public:
  interface::IMessagePtr createMessageByID(int id) override {
    switch (static_cast<EMessageType>(id)) {
      case EMessageType::KeyValue:return std::make_shared<KeyValue>();
      case EMessageType::PutOne:return std::make_shared<PutOne>();
      case EMessageType::WriteMany:return std::make_shared<WriteMany>();
      case EMessageType::RemoveOne:return std::make_shared<RemoveOne>();
      case EMessageType::RemoveRange:return std::make_shared<RemoveRange>();
    }
    return nullptr;
  }

  interface::IMessagePtr createMessageByName(const std::string &name) override {
    if (lookup_.count(name))
      return createMessageByID(lookup_[name]);
    return nullptr;
  }

  const std::string getProtocolName() const override { return "Avro"; }

  static AvroFactory *getInstance() {
    static AvroFactory fact;
    return &fact;
  }

  virtual ~AvroFactory() {}

 private:
  std::map<std::string, int> lookup_{
      {"KeyValue", static_cast<int>(EMessageType::KeyValue)},
      {"PutOne", static_cast<int>(EMessageType::PutOne)},
      {"WriteMany", static_cast<int>(EMessageType::WriteMany)},
      {"RemoveOne", static_cast<int>(EMessageType::RemoveOne)},
      {"RemoveRange", static_cast<int>(EMessageType::RemoveRange)},
  };
};

} /* namespace commonjson */
} /* namespace message */
} /* namespace lib */


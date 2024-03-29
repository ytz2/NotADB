#pragma once
#include <string>
#include <unordered_map>

#include "interface/message.h"
#include "message/commonjson/CommonJsonFactory.h"
#include "message/avro/AvroFactory.h"

namespace lib {
namespace message {
class MessageFactory {
 public:
  static MessageFactory *getInstance() {
    static MessageFactory fact;
    return &fact;
  }

  interface::IMessagePtr createMessageByProtocolMsgID(const std::string &name,
                                                      int id) {
    if (!factories_.count(name)) {
      return nullptr;
    }
    return factories_[name]->createMessageByID(id);
  }

  interface::IMessagePtr createMessageByProtocolMsgName(
      const std::string &name, const std::string &msgName) {
    if (!factories_.count(name)) {
      return nullptr;
    }
    return factories_[name]->createMessageByName(msgName);
  }

 private:
  MessageFactory() {
    auto jfactory = commonjson::CommonJsonFactory::getInstance();
    factories_[jfactory->getProtocolName()] = jfactory;
    auto afactory = avromsg::AvroFactory::getInstance();
    factories_[afactory->getProtocolName()] = afactory;
  }

  std::unordered_map<std::string, interface::IMessageFactory *> factories_;
};

} /* namespace message */
} /* namespace lib */

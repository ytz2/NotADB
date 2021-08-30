/* Copyright 2020 Yanhua Liu */
#ifndef SRC_MESSAGE_COMMONJSON_COMMONJSONFACTORY_H_
#define SRC_MESSAGE_COMMONJSON_COMMONJSONFACTORY_H_

#include <memory>
#include <string>

#include "interface/message.h"
#include "message/commonjson/CommonJsonMessage.h"

namespace lib {
namespace message {
namespace commonjson {

class CommonJsonFactory : public interface::IMessageFactory {
 public:
  interface::IMessagePtr createMessageByID(int id) override {
    return std::make_shared<CommonJsonMessage>();
  }

  interface::IMessagePtr createMessageByName(const std::string &name) override {
    return std::make_shared<CommonJsonMessage>();
  }

  const std::string getProtocolName() const { return "CommonJsonMessage"; }

  static CommonJsonFactory *getInstance() {
    static CommonJsonFactory fact;
    return &fact;
  }

  virtual ~CommonJsonFactory() {}
};

} /* namespace commonjson */
} /* namespace message */
} /* namespace lib */

#endif /* SRC_MESSAGE_COMMONJSON_COMMONJSONFACTORY_H_ */

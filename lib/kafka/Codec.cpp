//
// Created by Yanhua Liu on 9/14/21.
//
#include "Codec.h"
#include "message/MessageFactory.h"
namespace lib {
namespace kafka {

interface::IMessagePtr FlatMessageCodec::deserialize(const std::string &string) {
  int msgID = 0;
  return message::MessageFactory::getInstance()->createMessageByProtocolMsgID(
      protocol_, msgID);
}

bool FlatMessageCodec::serialize(interface::IMessagePtr msg, std::string &buffer) {
  buffer.clear();
  return msg->ToString(buffer);
}
}
}
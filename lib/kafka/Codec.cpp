//
// Created by Yanhua Liu on 9/14/21.
//
#include "Codec.h"
#include "message/MessageFactory.h"
namespace lib {
namespace kafka {

interface::IMessagePtr FlatMessageCodec::deserialize(const std::string &val) {
  int msgID = 0;
  auto msg = message::MessageFactory::getInstance()->createMessageByProtocolMsgID(
      protocol_, msgID);
  return msg && msg->FromString(val) ? msg : nullptr;
}

interface::IMessagePtr FlatMessageCodec::deserialize(const ::kafka::ConsumerRecord &record) {
  return deserialize(record.value().toString());
}

bool FlatMessageCodec::serialize(interface::IMessagePtr msg, std::string &buffer) {
  buffer.clear();
  return msg->ToString(buffer);
}
}
}
#include "Codec.h"
#include "message/MessageFactory.h"
#include <glog/logging.h>
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

interface::IMessagePtr AvroBinaryMessageCodec::deserialize(const std::string &val) {
  int msgID = 0;
  if (val.size() < 4)
    return nullptr;
  std::string idStr = val.substr(0, 4);
  try {
    msgID = std::stoi(idStr);
  } catch (...) {
    LOG(ERROR) << "unknown msg type " << idStr;
    return nullptr;
  }
  auto msg = message::MessageFactory::getInstance()->createMessageByProtocolMsgID(
      protocol_, msgID);
  return msg && msg->FromString(val.substr(4)) ? msg : nullptr;
}

interface::IMessagePtr AvroBinaryMessageCodec::deserialize(const ::kafka::ConsumerRecord &record) {
  return deserialize(record.value().toString());
}

// not that efficient , todo: migrate to boost buffer
bool AvroBinaryMessageCodec::serialize(interface::IMessagePtr msg, std::string &buffer) {
  buffer.clear();
  // reserve first 4 bytes as header
  // the first byte can be used for other encoding role, the other three bytes will be msg id
  // because kafka is handling the msg chopping, no need to encode msg len
  char buff[5];
  sprintf(buff, "%04d", msg->GetMessageID()); // yeah, ugly c, but thread safe
  std::string tmp;
  if (!msg->ToString(buffer))
    return false;
  buffer = std::string(buff) + tmp;
  return true;
}

}
}
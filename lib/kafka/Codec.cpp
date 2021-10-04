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
  std::string val((char *) record.value().data(), record.value().size());
  return deserialize(val);
}

bool FlatMessageCodec::serialize(interface::IMessagePtr msg, std::string &buffer) {
  buffer.clear();
  return msg->ToString(buffer);
}

interface::IMessagePtr AvroBinaryMessageCodec::deserialize(const std::string &val) {
  int msgID = 0;
  if (val.size() < 4) {
    LOG(ERROR) << "value " << val << " has invalid format";
    return nullptr;
  }
  std::string idStr = val.substr(0, 4);
  try {
    msgID = std::stoi(idStr);
  } catch (...) {
    LOG(ERROR) << "unknown msg type " << idStr;
    return nullptr;
  }
  auto msg = message::MessageFactory::getInstance()->createMessageByProtocolMsgID(
      protocol_, msgID);
  if (!msg) {
    LOG(ERROR) << "cannot create msg " << msgID << " for protocol " << protocol_;
  }
  auto token = val.substr(4);
  if (!msg->FromString(token)) {
    LOG(ERROR) << msg->GetMessageName() << " failed to deserialize " << token;
    return nullptr;
  }
  return msg;
}

interface::IMessagePtr AvroBinaryMessageCodec::deserialize(const ::kafka::ConsumerRecord &record) {
  std::string val((char *) record.value().data(), record.value().size());
  auto res = deserialize(val);
  decorate(record, res);
  return res;
}

void AvroBinaryMessageCodec::decorate(const ::kafka::ConsumerRecord &record, interface::IMessagePtr msg) {
  if (!msg || msg->GetMessageID() != static_cast<int>(lib::message::avromsg::EMessageType::Merge)) return;
  auto ptr = dynamic_cast<lib::message::avromsg::Merge *>(msg.get());
  if (!ptr) return;
  // merge operator needs the kafka meta info in record to avoid deliver at least once
  avrogen::MergeMeta meta;
  meta.topic = record.topic();
  meta.partition = record.partition();
  meta.offset = record.offset();
  ptr->meta.set_MergeMeta(meta);
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
  if (!msg->ToString(tmp))
    return false;
  buffer = std::string(buff) + tmp;
  return true;
}

}
}
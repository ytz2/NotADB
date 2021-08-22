/* Copyright 2020 Yanhua Liu */
#include "MessageCodec.h"

#include <glog/logging.h>
#include "message/MessageFactory.h"

#include <utility>
#include <vector>
namespace lib {
namespace tcp {

static int flat_header_length = 10;
static int flat_msgType_length = 4;
static int flat_msgLen_length = 6;
static int maxFlatMsgBufferSize = 10;
BufferPtr FlatMessageDecoder::writeBuffer(interface::IMessagePtr msg) {
  std::string tmp;
  if (!msg->ToString(tmp)) return nullptr;
  // encode header msgType and msgLength
  char header[flat_header_length + 1];
  memset(header, 0, flat_header_length + 1);
  sprintf(header, "%4d%6lu", msg->GetMessageID(), tmp.size());  // NOLINT
  tmp = std::string(header, 0, flat_header_length + 1) + tmp;
  writeMsgs_.push_back(std::move(tmp));
  if (writeMsgs_.size() > maxFlatMsgBufferSize) {
    writeMsgs_.pop_front();
  }
  const std::string &e = writeMsgs_.back();
  return std::make_shared<boost::asio::mutable_buffer>(
      const_cast<char *>(e.c_str()), e.size());
}

bool FlatMessageDecoder::getMessages(
    std::vector<interface::IMessagePtr> &msgs) {
  std::size_t s;
  auto data = const_cast<char *>(buffer_.getCurrentBuffer(s));
  while (s > flat_header_length) {
    try {
      int msgType = 0;
      int msgLen = 0;
      std::size_t size;
      msgType = std::stoi(std::string(data, 0, flat_msgType_length), &size, 10);
      msgLen =
          std::stoi(std::string(data, flat_msgType_length, flat_msgLen_length),
                    &size, 10);
      if (s - flat_header_length < msgLen) {
        break;
      }
      auto msg =
          message::MessageFactory::getInstance()->createMessageByProtocolMsgID(
              protocol, msgType);
      if (msg == nullptr) {
        return false;  // something wrong
      }
      if (!msg->FromString(std::string(data, flat_header_length, msgLen)))
        return false;
      msgs.push_back(msg);
      auto len = flat_header_length + msgLen;
      data += len;
      s -= len;
      buffer_.consume(len);
    } catch (const std::invalid_argument &e) {
      LOG(INFO) << e.what();
    }
  }

  return true;
}

} /* namespace tcp */
} /* namespace lib */

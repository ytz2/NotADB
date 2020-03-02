/* Copyright 2020 Yanhua Liu */
#pragma once

#include <interface/message.h>

#include <boost/asio.hpp>
#include <iostream>  //NOLINT
#include <list>      //NOLINT
#include <memory>    //NOLINT
#include <string>    //NOLINT

#include "MessageBuffer.h"

namespace lib {
namespace tcp {

enum TCP_PROTOCOL_TYPE {
  FLAT,    // header {msgType, size}[body]
  STREAM,  // eg. stream style, no header at all
  NONE
};

typedef std::shared_ptr<boost::asio::mutable_buffer> BufferPtr;
class MessageCodec {
 public:
  explicit MessageCodec(const std::string &protocol) : protocol(protocol) {}
  virtual ~MessageCodec() {}
  virtual std::shared_ptr<MessageCodec> clone() = 0;
  BufferPtr readBuffer(std::size_t size) {
    return std::make_shared<boost::asio::mutable_buffer>(
        buffer_.getBufferForRead(size), size);
  }

  void produce(std::size_t s) { buffer_.produce(s); }
  virtual BufferPtr writeBuffer(interface::IMessagePtr msg) = 0;
  virtual bool getMessages(
      std::vector<interface::IMessagePtr> &msgs) = 0;  // NOLINT

 protected:
  std::string protocol;
  std::list<std::string> writeMsgs_;
  MessageBuffer buffer_;
};
typedef std::shared_ptr<MessageCodec> MessageCodecPtr;
class FlatMessageDecoder : public MessageCodec {
 public:
  explicit FlatMessageDecoder(const std::string &protocol)
      : MessageCodec(protocol) {}
  virtual std::shared_ptr<MessageCodec> clone() {
    return std::make_shared<FlatMessageDecoder>(protocol);
  }
  BufferPtr writeBuffer(interface::IMessagePtr msg) override;
  bool getMessages(
      std::vector<interface::IMessagePtr> &msgs) override;  // NOLINT
};

} /* namespace tcp */
} /* namespace lib */

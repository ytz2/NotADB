/* Copyright 2020 Yanhua Liu */

#pragma once
#include <algorithm>
#include <array>
// here we assume message size is smaller than 1MB, consider to change this one
// for working with large size msg
#define MESSAGE_BUFFER_SIZE 1024 * 1024
namespace lib {
namespace tcp {

// a binary buffer for async read
class MessageBuffer {
 public:
  char* getBufferForRead(std::size_t size) {
    if (msgBufEnd_ + size < safeLast_) return data_ + msgBufEnd_;
    // we might read out of boundary, move memory
    auto relative = msgBufEnd_ - msgBufStart_;
    std::copy(data_ + msgBufStart_, data_ + msgBufEnd_, data_);
    msgBufStart_ = 0;
    msgBufEnd_ = relative;
    return data_ + msgBufEnd_;
  }
  const char* getCurrentBuffer(std::size_t& size) {  // NOLINT
    size = msgBufEnd_ - msgBufStart_;
    return data_ + msgBufStart_;
  }
  // consume number of chars, move the msgBufStart ahead
  void consume(std::size_t size) { msgBufStart_ += size; }

  // add more data into the buffer
  void produce(std::size_t size) { msgBufEnd_ += size; }

  char* getWriteBuffer() { return writeData_; }

 private:
  char data_[MESSAGE_BUFFER_SIZE];
  char writeData_[MESSAGE_BUFFER_SIZE];
  std::size_t msgBufStart_ = 0;
  std::size_t msgBufEnd_ = 0;
  const std::size_t safeLast_ =
      MESSAGE_BUFFER_SIZE - 1;  // add one more trailer
};

}  // namespace tcp
}  // namespace lib

/* Copyright 2020 Yanhua Liu */
#pragma once
// #include <boost/date_time/posix_time/posix_time.hpp>
#include <memory>  //NOLINT
#include <string>  // NOLINT

namespace interface {

class IMessage {
 public:
  virtual bool ToString(std::string &output) const = 0;  // NOLINT
  virtual bool FromString(const std::string &input) = 0;

  virtual int GetMessageID() = 0;

  virtual const std::string GetMessageName() = 0;

  virtual ~IMessage() {}

  virtual const std::string GetProtocol()  = 0;
};

typedef std::shared_ptr<IMessage> IMessagePtr;

class IMessageFactory {
 public:
  virtual IMessagePtr createMessageByID(int id) = 0;

  virtual IMessagePtr createMessageByName(const std::string &name) = 0;

  virtual const std::string getProtocolName() const = 0;
};

}  // namespace interface

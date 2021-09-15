/* Copyright 2020 Yanhua Liu */
#ifndef SRC_MESSAGE_COMMONJSON_COMMONJSONMESSAGE_H_
#define SRC_MESSAGE_COMMONJSON_COMMONJSONMESSAGE_H_

#include <json/json.h>
#include <string>

#include "interface/message.h"

namespace lib {
namespace message {
namespace commonjson {

class CommonJsonMessage : public interface::IMessage, public Json::Value {
 public:  // IMessage
  bool ToString(std::string &output) const override {
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    output = Json::writeString(builder, *this);
    return true;
  }

  bool FromString(const std::string &input) override {
    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();
    std::string errors;

    bool parsingSuccessful = reader->parse(
        input.c_str(), input.c_str() + input.size(), this, &errors);
    delete reader;
    return parsingSuccessful;
  }

  virtual int GetMessageID() override { return 0; }

  virtual const std::string GetMessageName() override { return "CommonJson"; }

  virtual const std::string GetProtocol() override { return "CommonJson"; };
};

} /* namespace commonjson */
} /* namespace message */
} /* namespace lib */

#endif  // SRC_MESSAGE_COMMONJSON_COMMONJSONMESSAGE_H_

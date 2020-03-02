/* Copyright 2020 Yanhua Liu */
#pragma once
#include <interface/message.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace lib {
namespace message {
namespace fix {

class FIXMessage : public interface::IMessage {
  typedef std::unordered_map<int, std::string> FIELDS;
  int GetMessageID() override { return id_; }
  const std::string GetMessageName() { return name_; }
  bool ToString(std::string &output) const override { return true; };  // NOLINT
  bool FromString(const std::string &input) override { return true; };

 private:
  int id_;            // msg id
  std::string name_;  // msg name
  FIELDS fields_;     // protocol defined fields
  std::unordered_map<int /*group field id*/,
                     std::vector<FIXMessage> /*repeated subobject*/>
      groups_;           // repeating group
  FIELDS customFields_;  // user defined fields
};

} /* namespace fix */
} /* namespace message */
} /* namespace lib */

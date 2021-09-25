#include <string>
#include "gtest/gtest.h"
#include "message/MessageFactory.h"


TEST(MessageTest, avro) {
  auto factory = lib::message::MessageFactory::getInstance();
  auto msg = factory->createMessageByProtocolMsgID("Avro", 0);
  EXPECT_TRUE(msg != nullptr);
  auto msg2 = factory->createMessageByProtocolMsgID("Avro", 0);
  EXPECT_TRUE(msg2 != nullptr);
  auto concreteMsg1 = std::dynamic_pointer_cast<lib::message::avromsg::KeyValue>(msg);
  auto concreteMsg2 = std::dynamic_pointer_cast<lib::message::avromsg::KeyValue>(msg2);
  EXPECT_TRUE(concreteMsg1 != nullptr && concreteMsg2 != nullptr);
  concreteMsg1->key = "123";
  concreteMsg1->value = "456";
  std::string tmp;
  EXPECT_TRUE(msg->ToString(tmp));
  EXPECT_TRUE(msg2->FromString(tmp));
  EXPECT_TRUE(concreteMsg1->key == concreteMsg2->key);
  EXPECT_TRUE(concreteMsg1->value == concreteMsg2->value);
}

TEST(MessageTest, json) {
  auto factory = lib::message::MessageFactory::getInstance();
  auto msg = factory->createMessageByProtocolMsgID("CommonJsonMessage", 0);
  EXPECT_TRUE(msg != nullptr);
  auto msg2 = factory->createMessageByProtocolMsgID("CommonJsonMessage", 0);
  EXPECT_TRUE(msg2 != nullptr);
  auto concreteMsg1 = std::dynamic_pointer_cast<lib::message::commonjson::CommonJsonMessage>(msg);
  auto concreteMsg2 = std::dynamic_pointer_cast<lib::message::commonjson::CommonJsonMessage>(msg2);
  EXPECT_TRUE(concreteMsg1 != nullptr && concreteMsg2 != nullptr);
  (*concreteMsg1)["key"] = "123";
  std::string tmp;
  EXPECT_TRUE(msg->ToString(tmp));
  EXPECT_TRUE(msg2->FromString(tmp));
  std::string tmp2 = (*concreteMsg2)["key"].asString();;
  EXPECT_TRUE(tmp2 == std::string("123"));
}

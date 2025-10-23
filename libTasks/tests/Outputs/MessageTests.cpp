#include <Task/Outputs/Message.hpp>

#include <gtest/gtest.h>

class MessageTest : public ::testing::Test
{};

TEST_F(MessageTest, DefaultConstructor)
{
    Outputs::Message message{};
    auto json = message.toJson();

    EXPECT_EQ(json["code"], 0);
    EXPECT_EQ(json["type"], "Info");
    EXPECT_EQ(json["text"], "<Success>: Расчет выполнен успешно.");
}

TEST_F(MessageTest, ToJson)
{
    Outputs::Message message{1, "Error", "Text"};
    auto json = message.toJson();

    EXPECT_EQ(json["code"], 1);
    EXPECT_EQ(json["type"], "Error");
    EXPECT_EQ(json["text"], "Text");
}

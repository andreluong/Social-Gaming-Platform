// #include "../web-socket-networking/
// #include "cha
#include "ChatClientLogic.h"
#include "gtest/gtest.h"
#include <optional>
#include <string>

using namespace testing;

class ProcessServerMessageTest : public::testing::Test {
protected:
    ClientData clientData;
    std::vector<std::string> messages;

    void SetUp() override {
        clientData.connectionId = "";
        clientData.name = std::nullopt;
        messages = {};
    }
};

TEST_F(ProcessServerMessageTest, WelcomeUser) {
    messages = {"Server>", "welcome" ,"123456"};
    processServerMessages(messages, clientData);
    EXPECT_EQ(clientData.connectionId, "123456");
}

TEST_F(ProcessServerMessageTest, WelcomeUserWithPreviousConnection) {
    clientData.connectionId = "12345";
    messages = {"Server>", "welcome" ,"67890"};
    processServerMessages(messages, clientData);
    EXPECT_EQ(clientData.connectionId, "67890");
}

TEST_F(ProcessServerMessageTest, RenameUnnamedUser) {
    clientData.connectionId = "12345";
    messages = {"Server>", "12345" ,"renamed", "to", "Bob"};
    processServerMessages(messages, clientData);
    EXPECT_EQ(clientData.name, "Bob");
}

TEST_F(ProcessServerMessageTest, RenameNamedUser) {
    clientData.connectionId = "12345";
    clientData.name = "Bob";
    messages = {"Server>", "Bob" ,"renamed", "to", "John"};
    processServerMessages(messages, clientData);
    EXPECT_EQ(clientData.name, "John");
}

TEST_F(ProcessServerMessageTest, NotRenameIfNameIsDifferent) {
    clientData.connectionId= "12345";
    messages = {"Server>", "67890" ,"renamed", "to", "Bob"};
    processServerMessages(messages, clientData);
    EXPECT_EQ(clientData.name, std::nullopt);
}


class GetMessageTypeTest : public::testing::Test {
protected:
    ClientData clientData;
    std::vector<std::string> messages;

    void SetUp() override {
        clientData.connectionId = "";
        clientData.name = std::nullopt;
        messages = {};
    }
};

TEST_F(GetMessageTypeTest, MessageTypeSelfWithNameUndefined) {
    clientData.connectionId = "12345";
    messages = {"12345>", "Hello!"};
    auto messageType = getMessageType(messages, clientData);
    EXPECT_EQ(messageType, MessageType::Self);
}

TEST_F(GetMessageTypeTest, MessageTypeSelfWithNameDefined) {
    clientData.connectionId = "12345";
    clientData.name = "John";
    messages = {"John>", "Hello!"};
    auto messageType = getMessageType(messages, clientData);
    EXPECT_EQ(messageType, MessageType::Self);
}

TEST_F(GetMessageTypeTest, MessageTypeServer) {
    messages = {"Server>", "Hello!"};
    auto messageType = getMessageType(messages, clientData);
    EXPECT_EQ(messageType, MessageType::Server);
}

TEST_F(GetMessageTypeTest, MessageTypeOther) {
    clientData.name = "John";
    messages = {"12345>", "Hello!"};
    auto messageType = getMessageType(messages, clientData);
    EXPECT_EQ(messageType, MessageType::Other);
}
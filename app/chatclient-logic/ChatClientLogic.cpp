#include "include/ChatClientLogic.h"

void processServerMessages(const std::vector<std::string>& messages, ClientData& clientData) {
  if (messages.size() <= 2 || messages[0] != "Server>") {
      return;
  }

  // message will only be given to a new user
  if (messages[1] == "welcome") {
    clientData.connectionId = messages[2];
  }

  // Server> a renamed to b - each name always 1 word
  if (messages.size() >= 5 && messages[2] == "renamed") {
    auto name = clientData.name.has_value() ? clientData.name.value() : clientData.connectionId;
    if (messages[1] == name) {
      clientData.name = messages[4];
    }
  }
}

enum MessageType getMessageType(const std::vector<std::string>& messages, const ClientData& clientData) {
  // lobby: 1 name> into name
  if (messages[0] == "lobby:") {
    return getMessageType(std::vector<std::string>(messages.begin() + 2, messages.end()), clientData);
  }
  if (messages[0] == "Server>") {
    return MessageType::Server;
  }
  if (clientData.name.has_value() && messages[0] == clientData.name.value() + ">") {
    return MessageType::Self;
  }
  if (!clientData.name.has_value() && messages[0] == clientData.connectionId + ">") {
    return MessageType::Self;
  }
  return MessageType::Other;
}

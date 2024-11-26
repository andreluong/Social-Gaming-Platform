/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <optional>
#include <unistd.h>
#include "ChatWindow.h"
#include "Client.h"

#include <vector>
#include <sstream> //

struct ClientData {
  std::string connectionId;
  std::optional<std::string> name = std::nullopt;
};

std::vector<std::string> splitStringBySpace(const std::string& str) {
  std::vector<std::string> result;
  std::istringstream stream(str);
  std::string word;

  // Extract each word separated by spaces
  while (stream >> word) {
      result.push_back(word);
  }

  return result;
};

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
    clientData.name = messages[4];
  }
}

enum MessageType getMessageType(const std::vector<std::string>& messages, const ClientData& clientData) {
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


int
main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: \n  " << argv[0] << " <ip address> <port>\n"
              << "  e.g. " << argv[0] << " localhost 4002\n";
    return 1;
  }

  networking::Client client{argv[1], argv[2]};
  ClientData clientData = {"", std::nullopt};

  bool done = false;
  auto onTextEntry = [&done, &client] (std::string text) {
    if ("exit" == text || "quit" == text) {
      done = true;
    } else {
      client.send(text);
    }
  };

  ChatWindow chatWindow(onTextEntry);

  while (!done && !client.isDisconnected()) {
    try {
      client.update();
    } catch (std::exception& e) {
      chatWindow.displayText("Exception from Client update:", MessageType::Server);
      chatWindow.displayText(e.what(), MessageType::Error);
      done = true;
    }

    auto response = client.receive();
    if (!response.empty()) {
      auto messages = splitStringBySpace(response);
      processServerMessages(messages, clientData);
      auto messageType = getMessageType(messages, clientData);
      chatWindow.displayText(response, messageType);
    }
    chatWindow.update();
  }

  return 0;
}


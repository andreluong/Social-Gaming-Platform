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
#include "string_utils.h"

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


/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////

#include "Server.h"
#include "User.h"
#include "Lobby.h"
#include "LobbyManager.h"
#include "string_utils.h"
#include "featureToggle.h"
#include <spdlog/spdlog.h>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include "inputRequestQueue.h"
#include "humanInput.h"
#include "humanInputType.h"

using networking::Connection;
using networking::Message;
using networking::Server;

LobbyManager lobbymanager;

auto inputRequestQueue = InputRequestQueue();

static auto findUser = [](uintptr_t connectionID) {
  auto f = [=](const User &user) { return user.getId() == connectionID; };
  // std::cout<<"i used the findUser function"<<std::endl;
  return f;
};

void onConnect(Connection c) {
  std::cout << "New connection found: " << c.id << "\n";
  lobbymanager.createUser(c.id, c);
}

void onDisconnect(Connection c) {
  std::cout << "Connection lost: " << c.id << "\n";
  lobbymanager.deleteUser(c.id);
}

struct processedMessage {
  std::string msg;
  uint lobbyNum;
};

struct MessageResult {
  std::vector<processedMessage> result;
  bool shouldShutdown;
};

// Compares users from the inputRequestQueue with a message connection
// If theres a match, add the message as a response for the user for an input request
void processInputRequestQueue(InputRequestQueue inputRequestQueue, const Message &message, std::ostringstream &result) {
  auto requestOpt = inputRequestQueue.getRequestFromMessage(message);
  
  // Saves response to user and removes request from inputRequestQueue
  if (requestOpt.has_value()) {
    auto requestIt = requestOpt.value();
    auto owner = requestIt->first;
    auto inputType = requestIt->second;
    owner.addResponse(message, inputType);
    inputRequestQueue.removeRequest(requestIt);
    result << "User chooses " << message.text << " as their response for input type:" << inputType << "\n";

  // No user owns message => throw error?
  } else {
    result << "ERROR: No user owns the message: " << message.text << "\n";
  }
};

void handleLobbyOperation(Server &server, const Message &message,
                          std::ostringstream &result,
                          const std::vector<std::unique_ptr<User>>::iterator &user, bool &quit) {
  auto lobbyid = user->get()->getLobby();
  auto username = user->get()->getName();
  // Quits both the lobby and the game
  std::pair<std::string, std::string> userInput = splitCommand(message.text);
  if (message.text == "quit") {
    lobbymanager.deleteUser(user->get()->getId());
    // Assuming `server` is accessible here
    server.disconnect(message.connection);
  } 
  else if (message.text == "SVshutdown") {
    std::cout << "Shutting down.\n";
    quit = true;
  } 
  else if (userInput.first == "rename" && userInput.second.length() > 0) {
    // playerIdToUsernameMap.insert_or_assign(message.connection.id,
    // userInput.second);
    auto userInputName = splitStringBySpace(userInput.second);
    result << "Server> " << user->get()->getName() << " renamed to " << userInputName[0]
           << "\n";
    user->get()->setName(userInputName[0]);
  } 
  else if (message.text == "leave") {
    lobbymanager.leaveLobby(user->get()->getId());
    result << "lobby: " << lobbyid << " " << username << "> " << message.text
           << "\n";
    result << "leaving lobby " << message.text << "\n";
  }
  else if(message.text == "delete"){
    lobbymanager.deleteLobby(user->get()->getLobby());
    result << "lobby: " << lobbyid << " " << username << "> " << message.text
           << "\n";
    result << "lobby deleted #" << lobbyid << "\n";
  }
  // Process user response to an input request if queue is not empty
  else if (!inputRequestQueue.isEmpty()) {
    processInputRequestQueue(inputRequestQueue, message, result);
  } 
  else {
    result << "lobby: " << lobbyid << " " << username << "> " << message.text
           << "\n";
  }
};

void handleNonLobbyOperation(const Message &message, std::ostringstream &result,
                             const std::vector<std::unique_ptr<User>>::iterator &user,
                             bool &quit) {
  std::pair<std::string, std::string> userInput = splitCommand(message.text);

  if (message.text == "quit") {
    lobbymanager.deleteUser(user->get()->getId());
    // Assuming `server` is accessible here
    // server.disconnect(message.connection);
  } 
  else if (message.text == "SVshutdown") {
    std::cout << "Shutting down.\n";
    quit = true;
  } 
  else if (userInput.first == "rename" && userInput.second.length() > 0) {
    auto userInputName = splitStringBySpace(userInput.second);
    result << "Server> " + user->get()->getName() << " renamed to " << userInputName[0] << "\n";
    user->get()->setName(userInputName[0]);
  } 
  else if (message.text == "create") {
    unsigned int lobbyNum = lobbymanager.createLobby(user->get()->getId());
    result << user->get()->getName() << "> " << message.text << "\n";
    result << "creating lobby " << lobbyNum << "\n";
  } 
  else if (std::all_of(message.text.begin(), message.text.end(), ::isdigit)) {
    JoinResult joinResult = lobbymanager.joinLobby((uint)std::stol(message.text), user->get()->getId());
    if(joinResult == JoinResult::SUCCESS){
      result << user->get()->getName() << "> " << message.text << "\n";
      result << "joining lobby #" << std::stoi(message.text) << "\n";
    }
    else if(joinResult == JoinResult::LOBBYFULL){
      result << user->get()->getName() << "> " << message.text << "\n";
      result << "lobby #" << std::stoi(message.text) << " is full\n";
    }
    else if(joinResult == JoinResult::DNE){
      result << user->get()->getName() << "> " << message.text << "\n";
      result << "lobby #" << std::stoi(message.text) << " does not exist\n";
    }
  } 
  else {
    result << user->get()->getName() << "> " << message.text << "\n";
  }
};

MessageResult processMessages(Server &server,
                              const std::deque<Message> &incoming) {
  bool quit = false;
  std::vector<processedMessage> results;

  for (const auto &message : incoming) {
    std::ostringstream result;
    auto user = lobbymanager.findUserIt(message.connection.id);

    // checking if the user lobby is not referencing the reception
    if (user->get()->getLobby() != 0) {
      handleLobbyOperation(server, message, result, user, quit);
    } else {
      handleNonLobbyOperation(message, result, user, quit);
    }

    results.push_back(
        processedMessage{result.str(), user->get()->getLobby()});
  }
  return MessageResult{results, quit};
}

std::deque<Message> buildOutgoing(const std::vector<processedMessage> &logs) {
  std::deque<Message> outgoing;
  for (auto log : logs) {
    for (auto& client : lobbymanager.getUsers()) {
      // auto user = std::find_if(users.begin(), users.end(), findUser(client.get()->getId()));
      auto user = lobbymanager.findUserIt(client.get()->getId());
      if (user->get()->getLobby() == log.lobbyNum) {
        outgoing.push_back({client.get()->getConnection(), log.msg});
      }
    }
  }
  return outgoing;
}

std::string getHTTPMessage(const char *htmlLocation) {
  if (access(htmlLocation, R_OK) != -1) {
    std::ifstream infile{htmlLocation};
    return std::string{std::istreambuf_iterator<char>(infile),
                       std::istreambuf_iterator<char>()};
  }

  std::cerr << "Unable to open HTML index file:\n" << htmlLocation << "\n";
  std::exit(-1);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n"
              << "  e.g. " << argv[0] << " 4002 ./webchat.html\n";
    return 1;
  }
  FeatureToggle featureToggle;
  spdlog::set_level(spdlog::level::debug); // comment in or out for debug messages
  if (featureToggle.isEnabled("feature1")) {
    std::cout << "feature1 is toggled on" << std::endl;
  };

  const unsigned short port = std::stoi(argv[1]);


  unsigned long connectionId = -1;
  bool hasNewConnection = false;

  Server server{port, getHTTPMessage(argv[2]), [&connectionId, &hasNewConnection](Connection connection) {
    connectionId = connection.id;
    onConnect(connection);
    hasNewConnection = true;
  }, onDisconnect};




  while (true) {
    bool errorWhileUpdating = false;
    try {
      server.update();
    } catch (std::exception &e) {
      std::cerr << "Exception from Server update:\n"
                << " " << e.what() << "\n\n";
      errorWhileUpdating = true;
    }
    if (hasNewConnection) {
      const std::deque<Message> messages = {
        {Connection{connectionId}, "Server> welcome " + std::to_string(connectionId) + "\n"}
      };
      server.send(messages);
      hasNewConnection = false;
    }
    const auto incoming = server.receive();
    const auto [logs, shouldQuit] = processMessages(server, incoming);
    const auto outgoing = buildOutgoing(logs);
    server.send(outgoing);

    if (shouldQuit || errorWhileUpdating) {
      break;
    }

    sleep(1);
  }

  return 0;
}

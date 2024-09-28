/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "Server.h"
#include "lobby.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>


using networking::Server;
using networking::Connection;
using networking::Message;


std::vector<Connection> clients;

std::vector<int> lobbyIDs;
std::vector<int> players;
std::map<unsigned long int, int> playerIdToLobbyIdMap;
unsigned int lobbyCounter =  0;



void
onConnect(Connection c) {
  std::cout << "New connection found: " << c.id << "\n";
  clients.push_back(c);
}


void
onDisconnect(Connection c) {
  std::cout << "Connection lost: " << c.id << "\n";
  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));
}

struct processedMessage {
  std::string msg;
  int lobbyNum;
};


struct MessageResult {
  std::vector<processedMessage> result;
  bool shouldShutdown;
};


MessageResult
processMessages(Server& server, const std::deque<Message>& incoming) {
  bool quit = false;
  std::vector<processedMessage> results;


  for (const auto& message : incoming) {
    std::ostringstream result;

    if (playerIdToLobbyIdMap
  .find(message.connection.id) != playerIdToLobbyIdMap
  .end()){
      int lobbyid = playerIdToLobbyIdMap
    [message.connection.id];
      // quits both the lobby and the game
      if (message.text == "quit") {
        playerIdToLobbyIdMap
      .erase(message.connection.id);
        server.disconnect(message.connection);
      } else if (message.text == "SVshutdown") {
        std::cout << "Shutting down.\n";
        quit = true;
      // quits the lobby but not the game, can still join an existing lobby
      } else if (message.text == "leave") {
        playerIdToLobbyIdMap
      .erase(message.connection.id);
        result << "lobby: " << lobbyid << " " << message.connection.id << "> " << message.text << "\n";
        result << "leaving lobby " << message.text << "\n";
      } else {
        result << "lobby: " << lobbyid << " " << message.connection.id << "> " << message.text << "\n";
      }
    }
    else{
      if (message.text == "quit") {
        server.disconnect(message.connection);
      } else if (message.text == "SVshutdown") {
        std::cout << "Shutting down.\n";
        quit = true;
      } else if (message.text == "create") {
        ++lobbyCounter;
        playerIdToLobbyIdMap
      .insert(std::make_pair(message.connection.id, lobbyCounter));
        result << message.connection.id << "> " << message.text << "\n";
        result << "creating lobby " << lobbyCounter << "\n";
        // should add the prompt after
      } else if (std::all_of(message.text.begin(), message.text.end(), isdigit)) {
        playerIdToLobbyIdMap
      .insert(std::make_pair(message.connection.id, std::stoi(message.text)));
        result << message.connection.id << "> " << message.text << "\n";
        result << "joining lobby " << std::stoi(message.text) << "\n";
      } else {
        result << message.connection.id << "> " << message.text << "\n";
      }
    }

    if (playerIdToLobbyIdMap
  .find(message.connection.id) != playerIdToLobbyIdMap
  .end()){
      results.push_back(processedMessage{result.str(), playerIdToLobbyIdMap
    [message.connection.id]});
    }
    else{
      results.push_back(processedMessage{result.str(), 0});
    }
  }

  return MessageResult{results, quit};
}


std::deque<Message>
buildOutgoing(const std::vector<processedMessage>& logs) {
  std::deque<Message> outgoing;
  for (auto log : logs){
    for (auto client : clients) {
      if (playerIdToLobbyIdMap
    .find(client.id) != playerIdToLobbyIdMap
    .end()){
        if(playerIdToLobbyIdMap
      .at(client.id) == log.lobbyNum){
          outgoing.push_back({client, log.msg});
        }
      }
      else{ // no lobby
        outgoing.push_back({client, log.msg});
      }
    }
  }
  return outgoing;
}


std::string
getHTTPMessage(const char* htmlLocation) {
  if (access(htmlLocation, R_OK ) != -1) {
    std::ifstream infile{htmlLocation};
    return std::string{std::istreambuf_iterator<char>(infile),
                       std::istreambuf_iterator<char>()};

  }

  std::cerr << "Unable to open HTML index file:\n"
            << htmlLocation << "\n";
  std::exit(-1);
}


int
main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n"
              << "  e.g. " << argv[0] << " 4002 ./webchat.html\n";
    return 1;
  }

  const unsigned short port = std::stoi(argv[1]);
  Server server{port, getHTTPMessage(argv[2]), onConnect, onDisconnect};

  while (true) {
    bool errorWhileUpdating = false;
    try {
      server.update();
    } catch (std::exception& e) {
      std::cerr << "Exception from Server update:\n"
                << " " << e.what() << "\n\n";
      errorWhileUpdating = true;
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


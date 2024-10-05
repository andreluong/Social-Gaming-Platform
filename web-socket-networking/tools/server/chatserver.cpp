/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////

#include "Server.h"
#include "lobby.h"
#include "string_utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <map>
#include <cstdlib>
#include "User.h"
#include "humanInput.h"

using networking::Connection;
using networking::Message;
using networking::Server;

std::vector<Connection> clients;

std::vector<int> lobbyIDs;
std::vector<int> players;
std::map<unsigned long int, int> playerIdToLobbyIdMap;
std::map<unsigned long int, std::string> playerIdToUsernameMap;

unsigned int lobbyCounter = 0;

void onConnect(Connection c)
{
  std::cout << "New connection found: " << c.id << "\n";
  clients.push_back(c);
}

void onDisconnect(Connection c)
{
  std::cout << "Connection lost: " << c.id << "\n";
  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  clients.erase(eraseBegin, std::end(clients));
}

struct processedMessage
{
  std::string msg;
  int lobbyNum;
};

struct MessageResult
{
  std::vector<processedMessage> result;
  bool shouldShutdown;
};

void handleLobbyOperation(Server &server, const Message &message, std::ostringstream &result, const std::string &username, bool &quit)
{
  int lobbyid = playerIdToLobbyIdMap[message.connection.id];

  // Quits both the lobby and the game
  if (message.text == "quit")
  {
    playerIdToLobbyIdMap.erase(message.connection.id);
    // Assuming `server` is accessible here
    // server.disconnect(message.connection);
  }
  else if (message.text == "SVshutdown")
  {
    std::cout << "Shutting down.\n";
    quit = true;
  }
  else if (message.text == "leave")
  {
    playerIdToLobbyIdMap.erase(message.connection.id);
    result << "lobby: " << lobbyid << " " << username << "> " << message.text << "\n";
    result << "leaving lobby " << message.text << "\n";
  }
  // Process user response to an input request if queue is not empty
  else if (const auto inputRequestQueue = server.getInputRequestQueue(); !inputRequestQueue.empty()) {
    processInputRequestQueue(inputRequestQueue, message, result);
  }
  else
  {
    result << "lobby: " << lobbyid << " " << username << "> " << message.text << "\n";
  }
};

void handleNonLobbyOperation(const Message &message, std::ostringstream &result, const std::string &username, bool &quit)
{
  if (message.text == "quit")
  {
    // Assuming `server` is accessible here
    // server.disconnect(message.connection);
  }
  else if (message.text == "SVshutdown")
  {
    std::cout << "Shutting down.\n";
    quit = true;
  }
  else if (message.text == "create")
  {
    ++lobbyCounter;
    playerIdToLobbyIdMap.insert(std::make_pair(message.connection.id, lobbyCounter));
    result << username << "> " << message.text << "\n";
    result << "creating lobby " << lobbyCounter << "\n";
  }
  else if (std::all_of(message.text.begin(), message.text.end(), ::isdigit))
  {
    playerIdToLobbyIdMap.insert(std::make_pair(message.connection.id, std::stoi(message.text)));
    result << username << "> " << message.text << "\n";
    result << "joining lobby " << std::stoi(message.text) << "\n";
  }
  else
  {
    result << username << "> " << message.text << "\n";
  }
};

// Compares users from the inputRequestQueue with a message connection
// If theres a match, add the message as a response for the user for an input request
void processInputRequestQueue(std::unordered_map<User, HumanInputType> inputRequestQueue,
                              const Message &message,
                              std::ostringstream &result) 
{
  // Returns true if the map pair has the same connection id as message
  auto userOwnsMessage = [&](std::pair<User, HumanInputType> userInput) -> bool {
    userInput.first.getConnection().id == message.connection.id;
  };
  auto userIt = std::find_if(inputRequestQueue.begin(), inputRequestQueue.end(), userOwnsMessage);

  // Saves response to user and remove user from inputRequestQueue
  if (userIt != inputRequestQueue.end()) {
    User owner = userIt->first;
    owner.addResponse(message, userIt->second);
    inputRequestQueue.erase(userIt);
    result << "User chooses " << message.text << " as their response for input type:" << userIt->second << "\n";
  } 
  // No user owns message => throw error?
  else {
    result << "ERROR: No user owns the message: " << message.text << "\n";
  }
};

MessageResult
processMessages(Server &server, const std::deque<Message> &incoming)
{
  bool quit = false;
  std::vector<processedMessage> results;

  for (const auto &message : incoming)
  {
    std::ostringstream result;

    std::pair<std::string, std::string> userInput = splitCommand(message.text);
    const std::string username = playerIdToUsernameMap.count(message.connection.id) 
      ? playerIdToUsernameMap.at(message.connection.id) 
      : std::to_string(message.connection.id);

    // Rename command
    if (userInput.first == "rename" && userInput.second.length() > 0)
    {
      playerIdToUsernameMap.insert_or_assign(message.connection.id, userInput.second);
      result << username << " renamed to " << userInput.second << "\n";
    }
    // Lobby and Global operations
    else if (playerIdToLobbyIdMap
                 .find(message.connection.id) != playerIdToLobbyIdMap
                                                     .end())
    {
      handleLobbyOperation(server, message, result, username, quit);
    }
    else
    {
      handleNonLobbyOperation(message, result, username, quit);
    }

    if (auto found = playerIdToLobbyIdMap.find(message.connection.id); found != playerIdToLobbyIdMap.end())
    {
      results.push_back(processedMessage{result.str(), found->second});
    }
    else
    {
      results.push_back(processedMessage{result.str(), 0});
    }
  }

  return MessageResult{results, quit};
}

std::deque<Message>
buildOutgoing(const std::vector<processedMessage> &logs)
{
  std::deque<Message> outgoing;
  for (auto log : logs)
  {
    for (auto client : clients)
    {
      if (playerIdToLobbyIdMap
              .find(client.id) != playerIdToLobbyIdMap
                                      .end())
      {
        if (playerIdToLobbyIdMap
                .at(client.id) == log.lobbyNum)
        {
          outgoing.push_back({client, log.msg});
        }
      }
      else
      { // no lobby
        outgoing.push_back({client, log.msg});
      }
    }
  }
  return outgoing;
}

std::string
getHTTPMessage(const char *htmlLocation)
{
  if (access(htmlLocation, R_OK) != -1)
  {
    std::ifstream infile{htmlLocation};
    return std::string{std::istreambuf_iterator<char>(infile),
                       std::istreambuf_iterator<char>()};
  }

  std::cerr << "Unable to open HTML index file:\n"
            << htmlLocation << "\n";
  std::exit(-1);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage:\n  " << argv[0] << " <port> <html response>\n"
              << "  e.g. " << argv[0] << " 4002 ./webchat.html\n";
    return 1;
  }

  const unsigned short port = std::stoi(argv[1]);
  Server server{port, getHTTPMessage(argv[2]), onConnect, onDisconnect};

  while (true)
  {
    bool errorWhileUpdating = false;
    try
    {
      server.update();
    }
    catch (std::exception &e)
    {
      std::cerr << "Exception from Server update:\n"
                << " " << e.what() << "\n\n";
      errorWhileUpdating = true;
    }

    const auto incoming = server.receive();
    const auto [logs, shouldQuit] = processMessages(server, incoming);
    const auto outgoing = buildOutgoing(logs);
    server.send(outgoing);

    if (shouldQuit || errorWhileUpdating)
    {
      break;
    }

    sleep(1);
  }

  return 0;
}

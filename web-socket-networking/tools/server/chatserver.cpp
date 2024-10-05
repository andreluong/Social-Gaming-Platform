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
#include "../../../game-logic/include/User.h"
#include "../../../game-logic/User.cpp"

using networking::Connection;
using networking::Message;
using networking::Server;

std::vector<Connection> clients;

std::vector<User> users;
Lobby reception = Lobby();
std::vector<Lobby> lobs = {reception};


// std::map<unsigned long int, int> playerIdToLobbyIdMap;
// std::map<unsigned long int, std::string> playerIdToUsernameMap;


auto findUser = [](uintptr_t connectionID){
  auto f = [=](const User& user){return user.getId() == connectionID;};
  // std::cout<<"i used the findUser function"<<std::endl;
  return f;
};

void deleteIfEmptyLobby(Lobby *lobby){
  if (lobby->getUsers().size() == 0){
    auto found = std::find(lobs.begin(), lobs.end(), *lobby);
    lobs.erase(found);
  }
}

void onConnect(Connection c)
{
  std::cout << "New connection found: " << c.id << "\n";
  User user(c.id);

  user.setLobby(&reception);
  reception.addUser(&user);

  users.push_back(user);
  clients.push_back(c);
}

void onDisconnect(Connection c)
{
  std::cout << "Connection lost: " << c.id << "\n";
  auto eraseBegin = std::remove(std::begin(clients), std::end(clients), c);
  auto userErase = std::remove_if(users.begin(), users.end(), findUser(c.id));
  userErase.base()->getLobby()->removeUser(userErase.base());
  users.erase(userErase);
  clients.erase(eraseBegin, std::end(clients));
}

struct processedMessage
{
  std::string msg;
  uint lobbyNum;
};

struct MessageResult
{
  std::vector<processedMessage> result;
  bool shouldShutdown;
};

void handleLobbyOperation(const Message &message, std::ostringstream &result, const std::vector<User>::iterator &user, bool &quit)
{
  // int lobbyid = playerIdToLobbyIdMap[message.connection.id];
  // auto user = std::find_if(users.begin(), users.end(), findUser(message.connection.id));
  auto lobbyid = user.base()->getLobby()->getLobbyNum();
  auto username = user.base()->getName();
  // Quits both the lobby and the game
  std::pair<std::string, std::string> userInput = splitCommand(message.text);
  if (message.text == "quit")
  {
    users.erase(user);
    // Assuming `server` is accessible here
    // server.disconnect(message.connection);
  }
  else if (message.text == "SVshutdown")
  {
    std::cout << "Shutting down.\n";
    quit = true;
  }
  else if (userInput.first == "rename" && userInput.second.length() > 0)
  {
    // playerIdToUsernameMap.insert_or_assign(message.connection.id, userInput.second);
    user.base()->setName(userInput.second);
    result << user.base()->getName() << " renamed to " << userInput.second << "\n";
  }
  else if (message.text == "leave")
  {
    deleteIfEmptyLobby(user.base()->getLobby());
    user.base()->setLobby(&reception);
    result << "lobby: " << lobbyid << " " << username << "> " << message.text << "\n";
    result << "leaving lobby " << message.text << "\n";
  }
  else
  {
    result << "lobby: " << lobbyid << " " << username << "> " << message.text << "\n";
  }
};

void handleNonLobbyOperation(const Message &message, std::ostringstream &result, const std::vector<User>::iterator&user, bool &quit)
{
  std::pair<std::string, std::string> userInput = splitCommand(message.text);

  if (message.text == "quit")
  {
    users.erase(user);
    // Assuming `server` is accessible here
    // server.disconnect(message.connection);
  }
  else if (message.text == "SVshutdown")
  {
    std::cout << "Shutting down.\n";
    quit = true;
  }
  else if (userInput.first == "rename" && userInput.second.length() > 0)
  {
    // playerIdToUsernameMap.insert_or_assign(message.connection.id, userInput.second);
    result << user.base()->getName() << " renamed to " << userInput.second << "\n";
    user.base()->setName(userInput.second);
  }
  else if (message.text == "create")
  {
    lobs.push_back(Lobby());
    
    user.base()->setLobby(&lobs.back());
    result << user.base()->getName() << "> " << message.text << "\n";
    result << "creating lobby " << lobs.back().getLobbyNum() << "\n";
  }
  else if (std::all_of(message.text.begin(), message.text.end(), ::isdigit))
  {
    auto lobby = std::find_if(lobs.begin(), lobs.end(),
                          [message](const Lobby &l)
                          {
                            l.getLobbyNum() == (uint)std::stol(message.text);
                          });
    user.base()->setLobby(lobby.base());
    result << user.base()->getName() << "> " << message.text << "\n";
    result << "joining lobby " << std::stoi(message.text) << "\n";
  }
  else
  {
    result << user.base()->getName() << "> " << message.text << "\n";
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

    auto user = std::find_if(users.begin(), users.end(), findUser(message.connection.id));

    // const std::string username = playerIdToUsernameMap.count(message.connection.id) ? playerIdToUsernameMap.at(message.connection.id) : std::to_string(message.connection.id);

    // checking if the user lobby is not referencing the reception
    if (user.base()->getLobby() != &reception)
    {
      handleLobbyOperation(message, result, user, quit);
    }
    else
    {
      handleNonLobbyOperation(message, result, user, quit);
    }

    results.push_back(processedMessage{result.str(), user.base()->getLobby()->getLobbyNum()});
    
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
    auto user = std::find_if(users.begin(), users.end(), findUser(client.id));
      if (user.base()->getLobby()->getLobbyNum() == log.lobbyNum)
      {
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

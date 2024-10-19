#include "LobbyManager.h"
#include <algorithm>

auto findUser = [](uintptr_t connectionID) {
  auto f = [=](const User &user) { return user.getId() == connectionID; };
  return f;
};

LobbyManager::LobbyManager():
    reserveInc(10),
    reception(Lobby())
{
    // amount reserved can be changed later on
    users.reserve(reserveInc);
    lobbies.reserve(reserveInc);
}

void LobbyManager::createUser(uintptr_t cid, networking::Connection c)
{
    // preallocate to reduce allocation instances
    if(users.capacity() == users.size()){
        users.reserve(users.capacity() + reserveInc);
    }
    User user(cid, c);
    users.push_back(user);
    reception.addUser(&user);
    user.setLobby(&reception);
}
void LobbyManager::deleteUser(uintptr_t cid)
{
    auto userErase = std::remove_if(users.begin(), users.end(), findUser(cid));
    userErase.base()->getLobby()->removeUser(userErase.base()); //remove user is currently unimplemented
    users.erase(userErase);
}
void LobbyManager::createLobby()
{
    if(lobbies.capacity() == lobbies.size()){
        lobbies.reserve(lobbies.capacity() + reserveInc);
    }
    lobbies.emplace_back();
}

void LobbyManager::deleteLobby(Lobby *lobby)
{
    auto found = std::find(lobbies.begin(), lobbies.end(), *lobby);
    lobbies.erase(found);
}

void LobbyManager::deleteIfLobbyEmpty(Lobby *lobby)
{
    if (lobby->getUsers().size() == 0) {
        deleteLobby(lobby);
    }
}

Lobby* LobbyManager::findLobby()
{

}

unsigned int LobbyManager::getUserLobbyNum(uintptr_t cid)
{
    
}
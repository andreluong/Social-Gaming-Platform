#include "LobbyManager.h"
#include <algorithm>

static auto findUser = [](uintptr_t connectionID) {
  auto f = [=](const std::unique_ptr<User> &user) { return user.get()->getId() == connectionID; };
  return f;
};

auto findLob = [](unsigned int lobbyID){
    auto f = [=](const Lobby &lobby){ return lobby.getLobbyNum() == lobbyID; };
    return f;
};

LobbyManager::LobbyManager():
    reserveInc(10)
{
    // amount reserved can be changed later on
    users.reserve(reserveInc);
    lobbies.reserve(reserveInc);
    auto reception = std::make_unique<Lobby>();
    lobbies.push_back(std::move(reception));
}

void LobbyManager::createUser(uintptr_t cid, networking::Connection c)
{
    // preallocate to reduce allocation instances
    if(users.capacity() == users.size()){
        users.reserve(users.capacity() + reserveInc);
    }
    // std::unique_ptr<User> user(new User(cid, c));
    auto user = std::make_unique<User>(cid, c);

    // User user(cid, c);
    std::cout << user.get()->getLobby() << std::endl;
    users.push_back(std::move(user));
    lobbies[0]->addUser(cid);
}

// finds the user and their lobby, finds the lobby and removes them from it, remove user after
void LobbyManager::deleteUser(uintptr_t cid)
{
    auto userErase = std::remove_if(users.begin(), users.end(), findUser(cid));
    unsigned int lobbyNum = userErase.base()->get()->getLobby();
    auto currentLobby = std::find_if(lobbies.begin(), lobbies.end(), 
    [&lobbyNum](const std::unique_ptr<Lobby>& lobby){
        return lobby.get()->getLobbyNum() == lobbyNum;
    });
    currentLobby.base()->get()->removeUser(cid);
    
    users.erase(userErase);
}

const std::vector<std::unique_ptr<User>> 
&LobbyManager::getUsers() const {
    return users;
}

unsigned int LobbyManager::createLobby()
{
    if(lobbies.capacity() == lobbies.size()){
        lobbies.reserve(lobbies.capacity() + reserveInc);
    }
    auto lobby = std::make_unique<Lobby>();
    lobbies.push_back(std::move(lobby));
    return lobbies.back().get()->getLobbyNum();
}



std::vector<std::unique_ptr<Lobby>>::iterator 
LobbyManager::findLobby(unsigned int lobbyNum)
{
    auto lobbyIt = std::find_if(lobbies.begin(), lobbies.end(), 
    [&lobbyNum](const std::unique_ptr<Lobby> &lobby){
        return lobby.get()->getLobbyNum() == lobbyNum;
    });
    return lobbyIt;
}

void LobbyManager::deleteLobby(unsigned int lobbyNum){
    auto lobbyIt = std::remove_if(lobbies.begin(), lobbies.end(), 
    [&lobbyNum](const std::unique_ptr<Lobby> &lobby){
        return lobby.get()->getLobbyNum() == lobbyNum;
    });
    lobbies.erase(lobbyIt, lobbies.end());
    return;
}

void LobbyManager::deleteIfLobbyEmpty(unsigned int lobbyNum){
    auto lobby = findLobby(lobbyNum);
    if (lobby.base()->get()->getUsers().size() == 0) {
        deleteLobby(lobbyNum);
    }
}


// unsigned int LobbyManager::getUserLobbyNum(uintptr_t cid)
// {
//     return findUserIt(cid).base()->getLobby()->getLobbyNum();
// }

// Lobby* LobbyManager::getReception()
// {
//     return &reception;
// }

std::vector<std::unique_ptr<User>>::iterator 
LobbyManager::findUserIt(uintptr_t cid)
{
    return std::find_if(users.begin(), users.end(), findUser(cid));
}

// std::vector<Lobby>::iterator LobbyManager::findLobbyIt(unsigned int lobbyNum)
// {
//     return std::find_if(lobbies.begin(), lobbies.end(), findLob(lobbyNum));
// }
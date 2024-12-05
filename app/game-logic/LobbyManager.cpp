#include "LobbyManager.h"
#include <algorithm>

auto findUser = [](uintptr_t connectionID) {
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
    auto user = std::make_unique<User>(cid, c);
    users.push_back(std::move(user));
    lobbies[0]->addUser(cid);
}
// finds the user and their lobby, finds the lobby and removes them from it, remove user after
void LobbyManager::deleteUser(uintptr_t cid)
{
    auto userErase = std::remove_if(users.begin(), users.end(), findUser(cid));
    unsigned int lobbyNum = userErase->get()->getLobby();
    auto currentLobby = std::find_if(lobbies.begin(), lobbies.end(), 
    [&lobbyNum](const std::unique_ptr<Lobby>& lobby){
        return lobby.get()->getLobbyNum() == lobbyNum;
    });
    currentLobby->get()->removeUser(cid);
    
    users.erase(userErase);
}

// Takes in lobby# and connection id and places them inside the lobby
// Should check if there is room in the lobby to join before pushing the id in
JoinResult LobbyManager::joinLobby(unsigned int lobbyNum, uintptr_t cid){
    auto lobbyit = findLobby(lobbyNum);
    if(0){
        return JoinResult::LOBBYFULL;
    }
    else if(lobbyit == lobbies.end()){
        return JoinResult::DNE;
    }
    auto userit = findUserIt(cid);
    lobbyit->get()->addUser(cid);
    userit->get()->setLobby(lobbyNum);
    return JoinResult::SUCCESS;
}

// This sets the users lobby to the default which is 0
// Also deletes the lobby that is left if everyone has left
void LobbyManager::leaveLobby(uintptr_t cid){
    auto userit = findUserIt(cid);
    unsigned int lobbyNum = userit->get()->getLobby();
    auto lobbyit = findLobby(lobbyNum);
    lobbyit->get()->removeUser(cid);
    deleteIfLobbyEmpty(lobbyNum);

    userit->get()->setLobby(0);
}

const std::vector<std::unique_ptr<User>> 
&LobbyManager::getUsers() const {
    return users;
}

// cid is used to assign the creator of the lobby to the lobby
unsigned int LobbyManager::createLobby(uintptr_t cid)
{
    if(lobbies.capacity() == lobbies.size()){
        lobbies.reserve(lobbies.capacity() + reserveInc);
    }
    auto lobby = std::make_unique<Lobby>();
    lobby.get()->addUser(cid);
    lobbies.push_back(std::move(lobby));
    unsigned int lobbyNum = lobbies.back().get()->getLobbyNum();
    joinLobby(lobbyNum, cid);
    return lobbyNum; // return is only used for print
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
    if(lobbyIt == lobbies.end()){
        return;
    }

    // segfaults sometimes
    std::vector<unsigned int> lobbyUsers = lobbyIt->get()->getUsers();
    if(!lobbyUsers.empty()){
        std::for_each(lobbyUsers.begin(), lobbyUsers.end(), [&](unsigned int cid){
            auto user = findUserIt(cid);
            spdlog::debug("cid #{} is being moved to lobby 0", cid);
            if(user != users.end()){
                user->get()->setLobby(0);
            }
        });
    }
    lobbies.erase(lobbyIt, lobbies.end());
    return;
}

void LobbyManager::deleteIfLobbyEmpty(unsigned int lobbyNum){
    auto lobby = findLobby(lobbyNum);
    spdlog::debug("number of people in the lobby : {}", lobby->get()->getUsers().size());
    if (lobby->get()->getUsers().size() == 0 && lobby != lobbies.end()) {
        deleteLobby(lobbyNum);
    }
}

std::vector<std::unique_ptr<User>>::iterator 
LobbyManager::findUserIt(uintptr_t cid)
{
    return std::find_if(users.begin(), users.end(), findUser(cid));
}

#ifndef LOBBYMANAGER_H
#define LOBBYMANAGER_H

#include "Lobby.h"
class Lobby;
class User;

class LobbyManager{
    public:
        LobbyManager();
        ~LobbyManager() = default;

        // User operations
        void createUser(uintptr_t cid, networking::Connection c);
        void deleteUser(uintptr_t cid);
        const std::vector<std::unique_ptr<User>>& getUsers() const;
        std::vector<std::unique_ptr<User>>::iterator findUserIt(uintptr_t cid);

        // Lobby operations
        unsigned int createLobby();
        void deleteLobby(unsigned int lobbyNum);
        void deleteIfLobbyEmpty(unsigned int lobbyNum);
        std::vector<std::unique_ptr<Lobby>>::iterator findLobby(unsigned int lobbyNum);
        std::vector<Lobby>::iterator findLobbyIt(unsigned int lobbyNum);
        unsigned int getUserLobbyNum(uintptr_t cid);

    private:
        std::vector<std::unique_ptr<Lobby>> lobbies;
        std::vector<std::unique_ptr<User>> users;
        unsigned int reserveInc;
};

#endif
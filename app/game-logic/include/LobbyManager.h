#ifndef LOBBYMANAGER_H
#define LOBBYMANAGER_H

#include "Lobby.h"
class Lobby;
class User;

class LobbyManager{
    public:
        LobbyManager();
        ~LobbyManager() = default;
        void createUser(uintptr_t cid, networking::Connection c);
        void deleteUser(uintptr_t cid);
        void createLobby();
        void deleteLobby(Lobby *lobby); // general delete
        void deleteIfLobbyEmpty(Lobby *lobby); // called everytime someone leaves
        Lobby* findLobby();
        unsigned int getUserLobbyNum(uintptr_t cid);

    private:
        std::vector<Lobby> lobbies;
        Lobby reception; // this is the starting lobby onConnect
        std::vector<User> users;
        unsigned int reserveInc;
};

#endif  
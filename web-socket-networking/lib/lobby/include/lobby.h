#ifndef LOBBY_H
#define LOBBY_H

#include <string>
#include <iostream>
#include <vector>
#include "../../../../game-logic/include/User.h"

class Lobby{
    public:
        Lobby();
        ~Lobby();
        std::string inviteCode;

        void addUser(User* user);
        void removeUser(User* user);
        std::vector<User*> getUsers();
        unsigned int getLobbyNum() const;
    private:
        unsigned int lobbyNumber;
        char randLetter();
        int randDigit();
        std::string tokenGenerator();

        std::vector<User*> users;
};

#endif
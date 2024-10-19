#ifndef LOBBY_H
#define LOBBY_H

#include <string>
#include <iostream>
#include <vector>
#include "User.h"

class User;

class Lobby{
    public:
        Lobby();
        ~Lobby();
        std::string inviteCode;

        void addUser(User* user);
        void removeUser(User* user);
        std::vector<User*> getUsers();
        unsigned int getLobbyNum() const;

        bool operator==(const Lobby& other);
    private:
        unsigned int lobbyNumber;
        char randLetter();
        int randDigit();
        std::string tokenGenerator();

        std::vector<User*> users;
};

#endif
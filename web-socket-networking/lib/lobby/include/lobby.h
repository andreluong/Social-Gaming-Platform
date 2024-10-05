#ifndef LOBBY_H
#define LOBBY_H

#include <string>
#include <iostream>

class Lobby{
    public:
        Lobby();
        ~Lobby();
        unsigned int lobbyNumber;
        std::string inviteCode;

    private:
        char randLetter();
        int randDigit();
        std::string tokenGenerator();
};

#endif
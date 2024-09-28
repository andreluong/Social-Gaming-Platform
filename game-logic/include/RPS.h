#pragma once
#include "Game.h"
#include <map>

class RPS : public Game {
    private:
    std::vector<std::string> choices = {"Rock", "Paper", "Scissors"};
    std::map<std::string, std::string> rules;

    public:
    RPS(int id, int maxP);

    //not sure if we need all of this methods will update as we go
    void initGame();
    void playRound();
    void determineRoundWinner();
    void displayRoundResult();
    void resetGame();
};
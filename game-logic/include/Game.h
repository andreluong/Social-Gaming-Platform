#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "User.h"

class Game {
    protected: //protected so we don't need getters/setter for our subclasses
    std::string gameName;    
    int gameID;          
    std::vector<std::shared_ptr<User>> players;
    std::unordered_map<std::string, int> scoreBoard; 
    int maxPlayers;   
    int currentRound;                 
    std::string status;         

public:
    // Constructor for the game class takes in unique id, the game name and the max number of players
    Game(int gId, const std::string& gName, int maxNumberOfPlayers);

    // Favouring composition over inheritance
    // Virtual destructor for polymorphic deletion
    virtual ~Game();
    virtual void startGame();
     void addPlayer(std::shared_ptr<User> player);
    void removePlayer(int playerID);
    bool canStart() const;
    virtual void endGame();
    void updateScoreBoard(const std::string& playerName, int score);
    std::string getStatus() const;
    virtual void displayScoreBoard() const;
};
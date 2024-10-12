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
    bool addPlayer(std::shared_ptr<User> player);
    bool removePlayer(int playerID);
    bool canStart() const;
    virtual void endGame();
    void updateScoreBoard(const std::string& playerName, int score);
    std::string getStatus() const;
    virtual void displayScoreBoard() const;

    //for week 2
    bool isGameFull() const;
    int getCurrentRound() const;
    void resetGame();
    std::shared_ptr<User> findPlayerByID(int playerID) const;
    // bool isGameFull() const;  
    void nextRound();
    int getScoreOfPlayer(const std::string& playerName) const;
    bool isPlayerInGame(int playerID) const;
    std::vector<std::string> getPlayerNames() const;
};
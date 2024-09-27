#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class Game {
    protected: //protected so we don't need getters/setter for our subclasses
    //the game feilds, to be updated in the next iterations when requirements will be more clear
    std::string gameName; //stores the name of the game          
    int gameID;           // Unique id for the game

    std::vector<std::shared_ptr<Player>> players; // a vector to store the list of players
    // Scoreboard to track scores
    std::unordered_map<std::string, int> scoreBoard; 
    int maxPlayers;   // for max number of players
    int currentRound;                 // to store the current round in the game
    std::string status;               // the active status of the game

public:
    // Constructor for the game class takes in unique id, the game name and the max number of players
    Game(int gId, const std::string& gName, int maxNumberOfPlayers);

    // Favouring composition over inheritance
    // Virtual destructor for polymorphic deletion
    virtual ~Game();
    virtual void startGame();
    void addPlayer(std::shared_ptr<Player> player);
    void removePlayer(int playerID);
    bool canStart() const;
    virtual void endGame();
    void updateScoreBoard(const std::string& playerName, int score);
    std::string getStatus() const;
    virtual void displayScoreBoard() const;
};
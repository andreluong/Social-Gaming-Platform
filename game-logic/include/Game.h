#pragma once
#include <string>




class Game {
    //the game feilds, to be updated in the next iterations when requirements will be more clear
    std::string gameName; //stores the name of the game          
    int gameID;           // Unique id for the game

    std::vector<std::shared_ptr<Player>> players; // a vector to store the list of players
    // Scoreboard to track scores
    std::unordered_map<std::string, int> scoreBoard; 
    int maxPlayers;   // for max number of players
    int currentRound;                 // to store the current round in the game
    std::string status;               // the active status of the game
}
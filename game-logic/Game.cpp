#include "Game.h"
#include "User.h"

Game::Game(int id, const std::string& name, int maxP)
    : gameID(id), gameName(name), maxPlayers(maxP), currentRound(0), status("Idle") {}

Game::~Game() {}

void Game::startGame() {
    if (canStart()) {
        status = "In Progress";
        std::cout << "Game " << gameName << " has started!" << std::endl;
    } else {
        std::cout << "Cannot start the game, min 2 players required." << std::endl;
    }
}

void Game::addPlayer(std::shared_ptr<User> player) {
    if (players.size() < maxPlayers) {
        players.push_back(player);
        std::cout << player->getName() << " has joined the game!" << std::endl; // Now this will work
    } else {
        std::cout << "Game is full, cannot add more players." << std::endl;
    }
}


void Game::removePlayer(int playerID) {
    auto it = std::remove_if(players.begin(), players.end(),
                             [playerID](const std::shared_ptr<User>& p) {
                                 return p->getId() == playerID;
                             });
    if (it != players.end()) {
        std::cout << (*it)->getName() << " has left the game!" << std::endl;
        players.erase(it, players.end());
    }
}

bool Game::canStart() const {
    return players.size() >= 2;
}

void Game::endGame() {
    status = "Completed";
    std::cout << "Game " << gameName << " has ended!" << std::endl;
}

void Game::updateScoreBoard(const std::string& playerName, int score) {
    scoreBoard[playerName] += score;
}

std::string Game::getStatus() const {
    return status;
}
void Game::displayScoreBoard() const {
    std::cout << "Scoreboard for game " << gameName << ":" << std::endl;
    for (const auto& entry : scoreBoard) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    }

}
//we find if a game is full by comparing no players wtih max
bool Game::isGameFull() const {
    return players.size() >= maxPlayers;
}

//to get the current game round
int Game::getCurrentRound() const {
    return currentRound;
}

//fun to move us to the next round
void Game::nextRound() {
    currentRound++;
    std::cout << "Advancing to round " << currentRound << std::endl;
}
//for resetting the game
void Game::resetGame() {
    scoreBoard.clear();
    players.clear();
    currentRound = 0;
    status = "Not Playing";
    std::cout << "the game has been reset" << std::endl;
}
//fun to help us find a player by their id
std::shared_ptr<User> Game::findPlayerByID(int playerID) const {
    for (const auto& player : players) {
        if (player->getId() == playerID) {
            return player;
        }
    }
    return nullptr; 
}
//fun to help us check if a particular player is in a game
bool Game::isPlayerInGame(int playerID) const {
    return findPlayerByID(playerID) != nullptr;
}

int Game::getScoreOfPlayer(const std::string& playerName) const {
    auto it = scoreBoard.find(playerName);
    if (it != scoreBoard.end()) {
        return it->second;
    }
    return 0;
}

//another func to get a player by their name
std::vector<std::string> Game::getPlayerNames() const {
    std::vector<std::string> names;
    for (const auto& player : players) {
        names.push_back(player->getName());
    }
    return names;
}
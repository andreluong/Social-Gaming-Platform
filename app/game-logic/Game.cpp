#include "Game.h"
#include "User.h"
#include <algorithm>
#include <spdlog/spdlog.h>

Game::Game(int id, const std::string& name, int maxP)
    : gameID(id), gameName(name), maxPlayers(maxP), currentRound(0), status("Idle") {}

Game::~Game() {}

void Game::startGame() {
    if (canStart()) {
        status = "In Progress";
        spdlog::info("Game {} has started");
    } else {
        spdlog::warn("Cannot start the game. Minimum 2 players required");
    }
}

bool Game::addPlayer(std::shared_ptr<User> player) {
    if (players.size() < maxPlayers) {
        players.push_back(player);
        spdlog::info("Welcome {} to the game", player->getName());
        return true;
    } else {
        spdlog::warn("Sorry, the game {} is full");
        return false;
    }
}

bool Game::removePlayer(int playerID) {
    auto it = std::find_if(players.begin(), players.end(),
                           [playerID](const std::shared_ptr<User>& p) {
                               return p->getId() == playerID;
                           });
    
    if (it != players.end()) {
        std::string playerName = (*it)->getName();
        players.erase(it);
        spdlog::info("{} left the game", playerName);
        return true;
    } else {
        spdlog::warn("Cannot find player with ID {} in game", playerID);
        return false;
    }
}

bool Game::canStart() const {
    return players.size() >= 2;
}

void Game::endGame() {
    status = "Completed";
    spdlog::info("Game has ended");
}

void Game::updateScoreBoard(const std::string& playerName, int score) {
    if (score < 0 && scoreBoard[playerName] + score < 0) {
        spdlog::warn("Negative score would result for player {} in game", playerName);
        return;
    }
    scoreBoard[playerName] += score;
    spdlog::info("Updated score for {}: {}", playerName, scoreBoard[playerName]);
}

std::string Game::getStatus() const {
    return status;
}

void Game::displayScoreBoard() const {
    spdlog::info("Scoreboard for game");
    for (const auto& entry : scoreBoard) {
        spdlog::info("{}: {}", entry.first, entry.second);
    }
}

bool Game::isGameFull() const {
    return players.size() >= maxPlayers;
}

int Game::getCurrentRound() const {
    return currentRound;
}

void Game::nextRound() {
    currentRound++;
    spdlog::info("Advancing to round {}", currentRound);
}

void Game::resetGame() {
    scoreBoard.clear();
    players.clear();
    currentRound = 0;
    status = "Not Playing";
    spdlog::info("Game has been reset");
}

std::shared_ptr<User> Game::findPlayerByID(int playerID) const {
    for (const auto& player : players) {
        if (player->getId() == playerID) {
            return player;
        }
    }
    return nullptr; 
}

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

std::vector<std::string> Game::getPlayerNames() const {
    std::vector<std::string> names;
    for (const auto& player : players) {
        names.push_back(player->getName());
    }
    return names;
}


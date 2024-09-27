#include "Game.h";

Game::Game(int id, const std::string& name, int maxP)
    : gameID(id), gameName(name), maxPlayers(maxP), currentRound(0), status("Idle") {}

Game::~Game() {}

void Game::startGame() {}

void Game::addPlayer(std::shared_ptr<Player> player) {}

void Game::removePlayer(int playerID) {}

bool Game::canStart() const {}

void Game::endGame() {}

void Game::updateScoreBoard(const std::string& playerName, int score) {}

std::string Game::getStatus() const {
    return status;
}
oid Game::displayScoreBoard() const {}
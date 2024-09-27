#include "Game.h";

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

void Game::addPlayer(std::shared_ptr<Player> player) {
    if (players.size() < maxPlayers) {
        players.push_back(player);
        std::cout << player->getName() << " has joined the game!" << std::endl;
    } else {
        std::cout << "Game is full, cannot add more players." << std::endl;
    }
}

void Game::removePlayer(int playerID) {
    auto it = std::remove_if(players.begin(), players.end(),
                             [playerID](const std::shared_ptr<Player>& p) {
                                 return p->getID() == playerID;
                             });
    if (it != players.end()) {
        std::cout << (*it)->getName() << " has left the game!" << std::endl;
        players.erase(it, players.end());
    }
}

bool Game::canStart() const {}

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
oid Game::displayScoreBoard() const {
    std::cout << "Scoreboard for game " << gameName << ":" << std::endl;
    for (const auto& entry : scoreBoard) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    }

}
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../game-logic/include/Game.h"
#include "../game-logic/include/User.h"

#include <memory>

using namespace testing;

class GameTests : public testing::Test {
protected:
    std::shared_ptr<User> player1;
    std::shared_ptr<User> player2;
    Game game;

    GameTests() : game(1, "testing the game class", 2) {}

    void SetUp() override {
        player1 = std::make_shared<User>(1, "Kevin");
        player2 = std::make_shared<User>(2, "Alex");
    }
};

TEST_F(GameTests, AddPlayer) {
    game.addPlayer(player1);
    EXPECT_EQ(game.getPlayerNames().size(), 1);
    EXPECT_EQ(game.getPlayerNames()[0], "Kevin");
}

TEST_F(GameTests, CannotAddMoreThanMaxPlayers) {
    game.addPlayer(player1);
    game.addPlayer(player2);
    
    auto player3 = std::make_shared<User>(3, "Anders");
    game.addPlayer(player3);  

    EXPECT_EQ(game.getPlayerNames().size(), 2);
}

TEST_F(GameTests, RemovePlayer) {
    game.addPlayer(player1);
    game.addPlayer(player2);
    game.removePlayer(player1->getId());

    EXPECT_EQ(game.getPlayerNames().size(), 1);
    EXPECT_EQ(game.getPlayerNames()[0], "Alex");
}

TEST_F(GameTests, CanStartWithMinimumPlayers) {
    game.addPlayer(player1);
    game.addPlayer(player2);
    EXPECT_TRUE(game.canStart());
}

TEST_F(GameTests, CannotStartWithLessThanTwoPlayers) {
    game.addPlayer(player1);
    EXPECT_FALSE(game.canStart());
}

TEST_F(GameTests, StartGame) {
    game.addPlayer(player1);
    game.addPlayer(player2);

    game.startGame();
    EXPECT_EQ(game.getStatus(), "In Progress");
}

TEST_F(GameTests, EndGame) {
    game.addPlayer(player1);
    game.addPlayer(player2);

    game.startGame();
    game.endGame();
    EXPECT_EQ(game.getStatus(), "Completed");
}

//Game full test
TEST_F(GameTests, IsGameFull) {
    game.addPlayer(player1);
    EXPECT_FALSE(game.isGameFull());

    game.addPlayer(player2);
    EXPECT_TRUE(game.isGameFull());
}
//a test for the next round
TEST_F(GameTests, NextRound) {
    game.addPlayer(player1);
    game.addPlayer(player2);

    int initialRound = game.getCurrentRound();
    game.nextRound();

    EXPECT_EQ(game.getCurrentRound(), initialRound + 1);
}
// a test for resetting the game
TEST_F(GameTests, ResetGame) {
    game.addPlayer(player1);
    game.addPlayer(player2);
    game.nextRound();
    game.updateScoreBoard(player1->getName(), 10);

    game.resetGame();

    EXPECT_EQ(game.getPlayerNames().size(), 0);
    EXPECT_EQ(game.getCurrentRound(), 0);
    EXPECT_EQ(game.getStatus(), "Not Playing");
    EXPECT_EQ(game.getScoreOfPlayer(player1->getName()), 0);
}
//test for finding a playr by their id
TEST_F(GameTests, FindPlayerByID) {
    game.addPlayer(player1);
    game.addPlayer(player2);

    auto foundPlayer = game.findPlayerByID(player1->getId());
    EXPECT_EQ(foundPlayer->getName(), "Kevin");

    //we use non existing id
    auto notFoundPlayer = game.findPlayerByID(34532);  
    EXPECT_EQ(notFoundPlayer, nullptr);
}
//test for checking if a player is in a game
TEST_F(GameTests, IsPlayerInGame) {
    game.addPlayer(player1);

    // Check if player1 and 2 are in the game
    bool player1InGame = game.isPlayerInGame(player1->getId());
    EXPECT_TRUE(player1InGame);
    bool player2InGame = game.isPlayerInGame(player2->getId());
    EXPECT_FALSE(player2InGame);
}

//test for the scoreboard
TEST_F(GameTests, UpdateScoreBoard) {
    game.addPlayer(player1);
    game.updateScoreBoard(player1->getName(), 5);

    EXPECT_EQ(game.getScoreOfPlayer(player1->getName()), 5);

    game.updateScoreBoard(player1->getName(), 10);
    EXPECT_EQ(game.getScoreOfPlayer(player1->getName()), 15);
}


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


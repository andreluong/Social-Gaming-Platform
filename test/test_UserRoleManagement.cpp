#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../game-logic/include/User.h"

#include <memory>

using namespace testing;

class UserTests : public testing::Test {
protected:
    User user;

    // because it expects a default constructor; this should still be created for each test
    UserTests() : user(1, "John") {} 

    void SetUp() override {}
};

TEST_F(UserTests, AddRole) {
    auto player = std::make_shared<Player>();
    user.addRole(player);
    EXPECT_TRUE(user.hasRole(RoleType::Player));
}

TEST_F(UserTests, RemoveRole) {

    auto player = std::make_shared<Player>();
    user.addRole(player);
    user.removeRole(RoleType::Player);
    EXPECT_FALSE(user.hasRole(RoleType::Player));
}


TEST_F(UserTests, CanBeHostAndPlayer) {
    auto host = std::make_shared<Host>();
    auto player = std::make_shared<Player>();

    user.addRole(host);
    user.addRole(player);

    EXPECT_TRUE(user.hasRole(RoleType::Host));
    EXPECT_TRUE(user.hasRole(RoleType::Player));
}

TEST_F(UserTests, CanBeHostAndAudience) {
    auto host = std::make_shared<Host>();
    auto audience = std::make_shared<Audience>();

    user.addRole(host);
    user.addRole(audience);

    EXPECT_TRUE(user.hasRole(RoleType::Host));
    EXPECT_TRUE(user.hasRole(RoleType::Audience));
}

TEST_F(UserTests, CannotHaveBothPlayerAndAudience) {
    auto player = std::make_shared<Player>();
    auto audience = std::make_shared<Audience>();

    user.addRole(player);
    EXPECT_THROW(user.addRole(audience), std::runtime_error);
}

TEST_F(UserTests, SwapBetweenPlayerAndAudience) {
    auto player = std::make_shared<Player>();

    user.addRole(player);
    EXPECT_TRUE(user.hasRole(RoleType::Player));
    EXPECT_FALSE(user.hasRole(RoleType::Audience));

    // Swap from Player to Audience
    user.swapPlayerAudienceRoles();
    EXPECT_FALSE(user.hasRole(RoleType::Player));
    EXPECT_TRUE(user.hasRole(RoleType::Audience));
}


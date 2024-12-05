#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Lobby.h"
#include "User.h"

// Create a fixture for Lobby tests
class LobbyTests : public ::testing::Test {
protected:
    std::shared_ptr<Lobby> lobby;
    std::shared_ptr<User> user1;
    std::shared_ptr<User> user2;
    std::shared_ptr<User> user3;

    void SetUp() override {
        // separted loby and usr instantiation
        lobby = std::make_shared<Lobby>();

        user1 = std::make_shared<User>(1, "UserOne", networking::Connection{1});
        user2 = std::make_shared<User>(2, "UserTwo", networking::Connection{2});
        user3 = std::make_shared<User>(3, "UserThree", networking::Connection{3});
    }
};

// Test lobby creation and invite code generation
TEST_F(LobbyTests, LobbyCreationAndInviteCode) {
    std::string inviteCode = lobby->inviteCode;
     // Copy for additional check
    std::string inviteCodeCopy = inviteCode; 

    EXPECT_EQ(inviteCode.length(), 6);  
    EXPECT_FALSE(inviteCode.empty()); 

    EXPECT_EQ(inviteCode, inviteCodeCopy);  
}

// Test lobby number assignment with many lobbies
TEST_F(LobbyTests, MultipleLobbiesHaveUniqueNumbers) {
    auto lobby1 = std::make_shared<Lobby>();
    auto lobby2 = std::make_shared<Lobby>();

    // Check their lobby numbers
    unsigned int lobbyNum1 = lobby1->getLobbyNum();
    unsigned int lobbyNum2 = lobby2->getLobbyNum();

    EXPECT_NE(lobbyNum1, lobbyNum2);  

    EXPECT_GE(lobbyNum1, 0);
    EXPECT_GE(lobbyNum2, 0);
}

// Test adding users to the lobby
TEST_F(LobbyTests, AddUsersToLobby) {
    //0 in the beginning
    EXPECT_EQ(lobby->getUsers().size(), 0);  

    // Add user1 and check again
    lobby->addUser(user1.get()->getId());
    // then copy the users list
    std::vector<unsigned int> usersAfterFirstAdd = lobby->getUsers();  
    EXPECT_EQ(usersAfterFirstAdd.size(), 1);  
    //check
    EXPECT_EQ(usersAfterFirstAdd[0], user1->getId());  

    // Add user2 and check again
    lobby->addUser(user2.get()->getId());
    std::vector<unsigned int> usersAfterSecondAdd = lobby->getUsers();  
    EXPECT_EQ(usersAfterSecondAdd.size(), 2);  
    EXPECT_EQ(usersAfterSecondAdd[1], user2->getId());  
}

// Test adding multiple users in different order and checking the result
TEST_F(LobbyTests, AddUsersInDifferentOrder) {
    // Add users in different order
    lobby->addUser(user2.get()->getId());
    lobby->addUser(user3.get()->getId());
    lobby->addUser(user1.get()->getId());

    // Check the size and order
    std::vector<unsigned int> currentUsers = lobby->getUsers();
    EXPECT_EQ(currentUsers.size(), 3);  
    EXPECT_EQ(currentUsers[0], user2->getId());  
    EXPECT_EQ(currentUsers[1], user3->getId());  
    EXPECT_EQ(currentUsers[2], user1->getId());  
}

// Test equality operator for lobbies
TEST_F(LobbyTests, LobbyEqualityOperator) {
    // Create another lobby for testing
    Lobby anotherLobby;
    EXPECT_FALSE(*lobby == anotherLobby); 

    // Assign same lobby number to simulate equality
    anotherLobby = *lobby;
    EXPECT_TRUE(*lobby == anotherLobby); 
}

// Test invite code validity (length and alternating characters)
TEST_F(LobbyTests, InviteCodeIsValid) {
    std::string code = lobby->inviteCode;
    EXPECT_EQ(code.length(), 6);

    for (size_t i = 0; i < code.size(); ++i) {
        if (i % 2 == 0) {
            // Even index should be digit
            EXPECT_TRUE(std::isdigit(code[i]));  
        } else {
            // Odd index should be letter
            EXPECT_TRUE(std::isalpha(code[i]));  
        }
    }
}

// Test adding a user and checking if they are a player or audience
TEST_F(LobbyTests, AddUserWithRoleCheck) {
    lobby->addUser(user1.get()->getId());

    // Check that the user has no role initially
    EXPECT_FALSE(user1->hasRole(RoleType::Player));  

    // Assign a player role to user1 and check
    auto playerRole = std::make_shared<Player>();
    user1->addRole(playerRole);
    EXPECT_TRUE(user1->hasRole(RoleType::Player)); 
}

// Test adding multiple users and checking their roles after addition
TEST_F(LobbyTests, AddMultipleUsersWithRoleCheck) {
    lobby->addUser(user1.get()->getId());
    lobby->addUser(user2.get()->getId());

    // Assign roles to both users
    auto playerRole1 = std::make_shared<Player>();
    auto playerRole2 = std::make_shared<Player>();

    user1->addRole(playerRole1);
    user2->addRole(playerRole2);

    // Check that both users are players
    EXPECT_TRUE(user1->hasRole(RoleType::Player));
    EXPECT_TRUE(user2->hasRole(RoleType::Player));

    EXPECT_EQ(lobby->getUsers().size(), 2);
}

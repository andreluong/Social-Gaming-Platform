#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../game-logic/include/lobby.h"
#include "../game-logic/include/User.h"

// Create a fixture for Lobby tests
class LobbyTests : public ::testing::Test {
protected:
    std::shared_ptr<Lobby> lobby;
    std::shared_ptr<User> user1;
    std::shared_ptr<User> user2;
    std::shared_ptr<User> user3;

    void SetUp() override {
        lobby = std::make_shared<Lobby>();
        user1 = std::make_shared<User>(1, "UserOne", networking::Connection{1});
        user2 = std::make_shared<User>(2, "UserTwo", networking::Connection{2});
        user3 = std::make_shared<User>(3, "UserThree", networking::Connection{3});
    }

};
//the test cases
// Goal: test lobby creation and invite code generation
TEST_F(LobbyTests, LobbyCreationAndInviteCode) {
    std::string inviteCode = lobby->inviteCode;
    // 3 digits + 3 letters (repeated thrice)
    EXPECT_EQ(inviteCode.length(), 9);  
    EXPECT_FALSE(inviteCode.empty());

    std::string inviteCode2 = lobby->inviteCode;
    // The invite code should remain the same for the same lobby
    EXPECT_EQ(inviteCode, inviteCode2);  
}
// Test lobby number assignment
TEST_F(LobbyTests, LobbyNumberAssignment) {
    unsigned int lobbyNum = lobby->getLobbyNum();
    
    //to make sure the lobbyNum is +ve
    EXPECT_GE(lobbyNum, 0);
}

// Test adding users to the lobby
TEST_F(LobbyTests, AddUserToLobby) {
    //no usrs in the beginning
    EXPECT_EQ(lobby->getUsers().size(), 0); 

    lobby->addUser(user1);
    //now we expect 1 usr
    EXPECT_EQ(lobby->getUsers().size(), 1); 

    lobby->addUser(user2);
    //now we expect 2 usrs
    EXPECT_EQ(lobby->getUsers().size(), 2); 
}

// Test removing users from the lobby
TEST_F(LobbyTests, RemoveUserFromLobby) {
    lobby->addUser(user1);
    lobby->addUser(user2);
    //chck both usrs addded
    EXPECT_EQ(lobby->getUsers().size(), 2); 

    lobby->removeUser(user1);
    // After removing one user, there should be 1 left
    EXPECT_EQ(lobby->getUsers().size(), 1);  
    // Check that the remaining user is user2
    EXPECT_EQ(lobby->getUsers().at(0)->getId(), user2->getId());  
}

// Test that users are actually removed from the lobby
TEST_F(LobbyTests, UserRemovalWorksCorrectly) {
    lobby->addUser(user1);
    lobby->addUser(user2);

    // Remove user1 and check
    lobby->removeUser(user1);
    auto users = lobby->getUsers();
    EXPECT_EQ(users.size(), 1);
    EXPECT_EQ(users[0]->getId(), user2->getId());

    // Remove user2 and check that the lobby is empty
    lobby->removeUser(user2);
    EXPECT_TRUE(lobby->getUsers().empty());
}

// Test getting users from the lobby
TEST_F(LobbyTests, GetUsersFromLobby) {
    lobby->addUser(user1);
    lobby->addUser(user2);

    std::vector<User*> users = lobby->getUsers();
    // There should be 2 users in the lobby
    EXPECT_EQ(users.size(), 2);  

    EXPECT_EQ(users[0]->getId(), user1->getId());
    EXPECT_EQ(users[1]->getId(), user2->getId());
}

// Test equality operator for lobbies
TEST_F(LobbyTests, LobbyEqualityOperator) {
    Lobby anotherLobby;
    // Lobbies with different IDs should not be equal
    EXPECT_FALSE(*lobby == anotherLobby);  

    // Manually assign the same lobby number for testing
    anotherLobby = *lobby;
    // Lobbies with the same ID should be equal
    EXPECT_TRUE(*lobby == anotherLobby);  
}

// Test lobby with multiple users
TEST_F(LobbyTests, AddMultipleUsersToLobby) {
    lobby->addUser(user1);
    lobby->addUser(user2);
    lobby->addUser(user3);

    std::vector<User*> users = lobby->getUsers();
     // There should be 3 users now
    EXPECT_EQ(users.size(), 3); 

    EXPECT_EQ(users[0]->getId(), user1->getId());
    EXPECT_EQ(users[1]->getId(), user2->getId());
    EXPECT_EQ(users[2]->getId(), user3->getId());
}

// Test lobby invite code generation logic
TEST_F(LobbyTests, InviteCodeIsValid) {
    std::string code = lobby->inviteCode;

     // Length should be 9 (3 parts: digit-letter pairs)
    EXPECT_EQ(code.length(), 9); 

    // Ensure it has alternating digits and letters
    for (size_t i = 0; i < code.size(); ++i) {
        if (i % 2 == 0) {
            EXPECT_TRUE(std::isdigit(code[i]));
        } else {
            EXPECT_TRUE(std::isalpha(code[i]));
        }
    }
}

// Test if lobby behaves correctly after many users are added and removed
TEST_F(LobbyTests, AddRemoveMultipleUsers) {
    lobby->addUser(user1);
    lobby->addUser(user2);
    lobby->addUser(user3);
    // expecting 3 users  in the lobby
    EXPECT_EQ(lobby->getUsers().size(), 3);  

    // Remove one user and verify the remaining users
    lobby->removeUser(user2);
    auto users = lobby->getUsers();
    // 2 users left
    EXPECT_EQ(users.size(), 2);  
    EXPECT_EQ(users[0]->getId(), user1->getId());
    EXPECT_EQ(users[1]->getId(), user3->getId());
}

// Test creating multiple lobbies and ensuring unique lobby numbers
TEST_F(LobbyTests, MultipleLobbiesHaveUniqueNumbers) {
    auto lobby1 = std::make_shared<Lobby>();
    auto lobby2 = std::make_shared<Lobby>();

    EXPECT_NE(lobby1->getLobbyNum(), lobby2->getLobbyNum());
}

#include "humanInput.h"

// This allows the player to make a multiple choice selection.
void inputChoice(Player& player, 
                std::string prompt, 
                std::vector<std::string> choices,
                std::string& target,
                int timeout,
                networking::Server* server) {
    // Server should send prompt and choices to player
    
    // TODO: Fix choices conversion to string
    std::string choicePrompt = prompt + ": " + choices.toString
    networking::Message msg { connection, choicePrompt };
    std::deque<Message> messages;
    messages.push_back(choicePrompt);
    networking::Server::send(messages);

    // TODO: Get input from player to be stored in target
}

#include "humanInput.h"

// This allows the player to make a multiple choice selection.
void inputChoice(Player& player, 
                std::string prompt, 
                std::vector<std::string> choices,
                std::string& target,
                int timeout,
                networking::Server* server) {
    // Server should send prompt and choices to player
    std::string allChoices;
    for (std::string x : choices) {
        allChoices.append(x + " ");
    }

    std::string choicePrompt = prompt + ": " + allChoices;
    networking::Message msg { networking::Connection{1}, choicePrompt }; // Temporary connection
    std::deque<networking::Message> messages;
    messages.push_back(msg);
    server->send(messages);

    // TODO: Get input from player to be stored in target
}

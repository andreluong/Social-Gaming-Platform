#include "humanInput.h"

// This allows the player to make a multiple choice selection.
void inputChoice(Player &player,
                 std::string prompt,
                 std::vector<std::string> choices,
                 std::string &target,
                 int timeout,
                 networking::Server *server)
{
    // Server should send prompt and choices to player
    std::string allChoices;
    for (std::string x : choices)
    {
        allChoices.append(x + " ");
    }

    std::string choicePrompt = prompt + ": " + allChoices;
    networking::Message msg{networking::Connection{1}, choicePrompt}; // Temporary connection
    std::deque<networking::Message> messages;
    messages.push_back(msg);
    server->send(messages);

    // TODO: Get input from player to be stored in target
}

void inputText(Player *player,
               std::string prompt,
               std::string &target,
               int timeout,
               networking::Server *server)
{
    networking::Message msg{networking::Connection{1}, prompt};
    std::deque<networking::Message> messages;
    messages.push_back(msg);
    server->send(messages);

    // TODO: Get the input from the text
}

void inputRange(Player *player,
                std::string prompt,
                std::pair<int, int> range,
                std::string &target,
                int timeout,
                networking::Server *server)
{
    std::string rangePrompt = "Range is between " + std::to_string(range.first) + " and " + std::to_string(range.second);
    std::string totalPrompt = prompt + "\n" + rangePrompt;
    networking::Message msg{networking::Connection{1}, prompt};
    std::deque<networking::Message> messages;
    messages.push_back(msg);
    server->send(messages);

    // TODO: Get the input from the range
}

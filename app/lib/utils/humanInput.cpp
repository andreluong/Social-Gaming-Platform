#include "humanInput.h"
#include <ranges>

// This allows the player to make a multiple choice selection.
void inputChoice(User *user,
                 std::string_view prompt,
                 std::vector<std::string_view> choices,
                 std::string_view &target,
                 int timeout,
                 Server *server)
{
    // const auto allChoices = choices | std::views::join_with('; ');
    // const std::string choicesPrompt = std::string(prompt) + ":\n" + allChoices;
    // const Message msg{ 0 /*user->getConnection()*/, choicesPrompt};
    
    // std::deque<Message> messages;
    // messages.push_back(msg);
    // server->send(messages);
    // server->addInputRequest(*user, CHOICE);
}

void inputText(User *user,
               std::string_view prompt,
               std::string_view &target,
               int timeout,
               Server *server)
{
    // const Message msg{0 /*user->getConnection()*/, std::string(prompt)};
    // std::deque<Message> messages;
    // messages.push_back(msg);
    // server->send(messages);
    // server->addInputRequest(*user, TEXT);
}

void inputRange(User *user,
                std::string_view prompt,
                std::pair<int, int> range,
                std::string_view &target,
                int timeout,
                Server *server)
{
    const std::string_view rangePrompt = "Range is between " + 
        std::to_string(range.first) + 
        " and " + 
        std::to_string(range.second);
    const std::string totalPrompt = std::string(prompt) + ":\n" + std::string(rangePrompt);
    // const Message msg{0 /*user->getConnection()*/, totalPrompt};

    // std::deque<Message> messages;
    // messages.push_back(msg);
    // server->send(messages);
    // server->addInputRequest(*user, RANGE);
}

void inputVote(User *user,
                std::string_view prompt,
                std::vector<std::string_view> choices,
                std::string &target,
                int timeout,
                Server *server) 
{
    // const auto allChoices = choices | std::views::join_with('; ');
    // const std::string choicesPrompt = std::string(prompt) + ":\n" + allChoices;
    // const Message msg{0 /*user->getConnection()*/, choicesPrompt};

    // std::deque<Message> messages;
    // messages.push_back(msg);
    // server->send(messages);
    // server->addInputRequest(*user, VOTE);
}
#include "humanInput.h"
#include "humanInputType.h"
#include <ranges>
#include <numeric>

// This allows the player to make a multiple choice selection.
void inputChoice(User *user,
                 std::string_view prompt,
                 std::vector<std::string_view> choices,
                 std::string_view &target,
                 int timeout,
                 networking::Server *server,
                 InputRequestQueue *queue)
{
    auto joinStringView = [](std::string str, std::string_view sv) {
        return std::move(str) + (str.empty() ? "" : " ") + std::string(sv);
    };
    auto allChoices = std::accumulate(choices.begin(), choices.end(), std::string{}, joinStringView);
    auto choicesPrompt = std::string(prompt) + ":\n" + allChoices;
    const networking::Message msg{ user->getConnection(), choicesPrompt};
    
    std::deque<networking::Message> messages;
    messages.push_back(msg);
    server->send(messages);
    queue->addInputRequest(*user, HumanInputType::CHOICE);
}

void inputText(User *user,
               std::string_view prompt,
               std::string_view &target,
               int timeout,
               networking::Server *server,
               InputRequestQueue *queue)
{
    const networking::Message msg{user->getConnection(), std::string(prompt)};
    std::deque<networking::Message> messages;
    messages.push_back(msg);
    server->send(messages);
    queue->addInputRequest(*user, HumanInputType::TEXT);
}

void inputRange(User *user,
                std::string_view prompt,
                std::pair<int, int> range,
                std::string_view &target,
                int timeout,
                networking::Server *server,
                InputRequestQueue *queue)
{
    const std::string_view rangePrompt = "Range is between " + 
        std::to_string(range.first) + 
        " and " + 
        std::to_string(range.second);
    const std::string totalPrompt = std::string(prompt) + ":\n" + std::string(rangePrompt);
    const networking::Message msg{user->getConnection(), totalPrompt};

    std::deque<networking::Message> messages;
    messages.push_back(msg);
    server->send(messages);
    queue->addInputRequest(*user, HumanInputType::RANGE);
}

void inputVote(User *user,
                std::string_view prompt,
                std::vector<std::string_view> choices,
                std::string &target,
                int timeout,
                networking::Server *server,
                InputRequestQueue *queue) 
{
     auto joinStringView = [](std::string str, std::string_view sv) {
        return std::move(str) + (str.empty() ? "" : " ") + std::string(sv);
    };
    auto allChoices = std::accumulate(choices.begin(), choices.end(), std::string{}, joinStringView);
    auto choicesPrompt = std::string(prompt) + ":\n" + allChoices;
    const networking::Message msg{user->getConnection(), choicesPrompt};

    std::deque<networking::Message> messages;
    messages.push_back(msg);
    server->send(messages);
    queue->addInputRequest(*user, HumanInputType::VOTE);
}
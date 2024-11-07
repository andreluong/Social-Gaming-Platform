#include "humanInput.h"
#include "humanInputType.h"
#include <ranges>
#include <numeric>

// Create and send a message to a server
static void sendMessage(User* user,
                        std::string prompt, 
                        networking::Server* server, 
                        InputRequestQueue* queue,
                        HumanInputType inputType) {
    std::deque<networking::Message> messages;
    const networking::Message msg{ user->getConnection(), prompt};
    messages.push_back(msg);
    server->send(messages);
    queue->addInputRequest(*user, inputType);
}

// Join vector of choices to a string
static std::string joinChoices(const std::vector<std::string_view>& choices) {
    auto joinStringView = [](std::string str, std::string_view sv) {
        return std::move(str) + (str.empty() ? "" : " ") + std::string(sv);
    };
    return std::accumulate(choices.begin(), choices.end(), std::string{}, joinStringView);
}

// This allows the player to make a multiple choice selection.
void inputChoice(User *user,
                 std::string_view prompt,
                 std::vector<std::string_view> choices,
                 std::string_view &target, // TODO: Figure out, user already has responses vector
                 int timeout,
                 networking::Server *server,
                 InputRequestQueue *queue)
{
    auto allChoices = joinChoices(choices);
    auto choicesPrompt = std::string(prompt) + ":\n" + allChoices; 
    sendMessage(user, choicesPrompt, server, queue, HumanInputType::CHOICE);
}

void inputText(User *user,
               std::string_view prompt,
               std::string_view &target,
               int timeout,
               networking::Server *server,
               InputRequestQueue *queue)
{
    sendMessage(user, std::string(prompt), server, queue, HumanInputType::TEXT);
}

void inputRange(User *user,
                std::string_view prompt,
                std::pair<int, int> range,
                std::string_view &target,
                int timeout,
                networking::Server *server,
                InputRequestQueue *queue)
{
    auto rangesPrompt = "Range is between" + std::to_string(range.first) +
                        " and " + std::to_string(range.second);
    auto totalPrompt = std::string(prompt) + ":\n" + rangesPrompt; 
    sendMessage(user, totalPrompt, server, queue, HumanInputType::RANGE);
}

void inputVote(User *user,
                std::string_view prompt,
                std::vector<std::string_view> choices,
                std::string &target,
                int timeout,
                networking::Server *server,
                InputRequestQueue *queue) 
{
    auto allChoices = joinChoices(choices);
    auto choicesPrompt = std::string(prompt) + ":\n" + allChoices; 
    sendMessage(user, choicesPrompt, server, queue, HumanInputType::VOTE);
}
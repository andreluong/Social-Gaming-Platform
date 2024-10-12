#pragma once

#include <vector>
#include "dataVariant.h"
#include "../../../game-logic/include/User.h"
#include <string_view>
#include <string>

enum HumanInputType {
    CHOICE,
    TEXT,
    RANGE,
    VOTE
};

// TODO: remove after correct linking
typedef int Server; 

void inputChoice(User* user, 
                std::string_view prompt, 
                std::vector<std::string_view> choices,
                std::string& target,
                int timeout,
                Server* server);

void inputText(User* user,
                std::string_view prompt,
                std::string& target,
                int timeout,
                Server* server);

void inputRange(User* user,
                std::string_view prompt,
                std::pair<int, int> range,
                std::string& target,
                int timeout,
                Server* server);

void inputVote(User* user,
                std::string_view prompt,
                std::vector<std::string_view> choices,
                std::string& target,
                int timeout,
                Server* server);

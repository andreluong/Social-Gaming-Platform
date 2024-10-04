#pragma once

#include <vector>
#include "dataVariant.h"
#include "Server.h"
#include "User.h"
#include <string_view>

enum HumanInputType {
    CHOICE,
    TEXT,
    RANGE,
    VOTE
};

void inputChoice(User* user, 
                std::string_view prompt, 
                std::vector<dataVariant> choices,
                std::string& target,
                int timeout,
                networking::Server* server);

void inputText(User* user,
                std::string_view prompt,
                std::string& target,
                int timeout,
                networking::Server* server);

void inputRange(User* user,
                std::string_view prompt,
                std::pair<int, int> range,
                std::string& target,
                int timeout,
                networking::Server* server);

void inputVote(User* user,
                std::string_view prompt,
                std::vector<dataVariant> choices,
                std::string& target,
                int timeout,
                networking::Server* server);

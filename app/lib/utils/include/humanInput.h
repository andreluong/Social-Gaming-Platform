#pragma once

#include <vector>
#include "dataVariant.h"
#include "User.h"
#include <string_view>
#include <string>
#include "Server.h"
#include "inputRequestQueue.h"

void inputChoice(User* user, 
                std::string_view prompt, 
                std::vector<std::string_view> choices,
                std::string& target,
                int timeout,
                networking::Server* server,
                InputRequestQueue* queue);

void inputText(User* user,
                std::string_view prompt,
                std::string& target,
                int timeout,
                networking::Server* server,
                InputRequestQueue* queue);

void inputRange(User* user,
                std::string_view prompt,
                std::pair<int, int> range,
                std::string& target,
                int timeout,
                networking::Server* server,
                InputRequestQueue* queue);

void inputVote(User* user,
                std::string_view prompt,
                std::vector<std::string_view> choices,
                std::string& target,
                int timeout,
                networking::Server* server,
                InputRequestQueue* queue);

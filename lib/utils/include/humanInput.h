#pragma once

#include <vector>
#include "dataVariant.h"
#include "Server.h"

typedef int Player; // Temporary

void inputChoice(Player* player, 
                std::string prompt, 
                std::vector<dataVariant> choices,
                std::string& target,
                int timeout,
                networking::Server* server);

void inputText(Player* player,
                std::string prompt,
                std::string& target,
                int timeout,
                networking::Server* server);

void inputRange(Player* player,
                std::string prompt,
                std::pair<int, int> range,
                std::string& target,
                int timeout,
                networking::Server* server);

void inputVote(Player* player,
                std::string prompt,
                std::vector<dataVariant> choices,
                std::string& target,
                int timeout,
                networking::Server* server);

void getInput(Player* player, std::string& target);
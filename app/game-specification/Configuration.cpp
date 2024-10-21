#include "Configuration.h"
#include <sstream>
#include <iostream>

Configuration::Configuration(const std::string& name, std::pair<int, int> playerRange, bool hasAudience)
    : name(name), playerRange(playerRange), hasAudience(hasAudience) {}

// Getters
std::string Configuration::getName() const {
    return name;
}

std::pair<int, int> Configuration::getPlayerRange() const {
    return playerRange;
}

bool Configuration::hasAudienceMembers() const {
    return hasAudience;
}

const std::vector<SetupRule>& Configuration::getSetupRules() const {
    return setupRules;
}

// Debug
void Configuration::printPlayerRange() {
        std::cout << "(" << playerRange.first << ", " << playerRange.second << ")" << std::endl;
    }

void Configuration::printHasAudience() {
        std::cout << std::boolalpha << hasAudience << std::endl;
    }

// Setters
void Configuration::setName(std::string_view n) {
    name = n;
}

// Expects input like "(2, 4)"
void Configuration::setPlayerRange(std::string_view playerRangeStr) {

    std::string str = std::string(playerRangeStr);
    if (str.front() == '(') str.erase(0, 1);
    if (str.back() == ')') str.pop_back();

    std::stringstream ss(str);
    std::string token;
    int first, second;

    if (std::getline(ss, token, ',')) {
        first = std::stoi(token);
    }
    if (std::getline(ss, token, ',')) {
        second = std::stoi(token);
    }

    playerRange = {first, second};
}

// Expects "true" or "false"
void Configuration::setHasAudience(std::string_view hasAudienceStr) {
    if (hasAudienceStr == "true") {
        hasAudience = true;
    } else if (hasAudienceStr == "false") {
        hasAudience = false;
    }
}

void Configuration::addSetupRule(const SetupRule& rule) {
    setupRules.push_back(rule);
}

#include "GameContext.h"
#include <iostream>

GameContext::GameContext(std::unordered_map<std::string, ExpressionVariant> configuration, 
                        std::unordered_map<std::string, ExpressionVariant> constants,
                        std::unordered_map<std::string, ExpressionVariant> variables,
                        std::unordered_map<std::string, ExpressionVariant> perPlayer,
                        std::unordered_map<std::string, ExpressionVariant> perAudience)
    : configuration(configuration),
    constants(constants), 
    variables(variables), 
    perPlayer(perPlayer), 
    perAudience(perAudience) {}

// Finds a value from all maps using a key
std::optional<ExpressionVariant> GameContext::find(std::string key) {
    // If found prefix, belongs to configuration, players, player, winners, weapons, or weapon
    auto prefixPos = key.find(".");
    if (prefixPos != std::string::npos) {
        auto prefix = key.substr(0, prefixPos);
        if (prefix == "configuration") {
            auto suffix = key.substr(prefixPos);
            if (suffix.find("rounds")) {
                return configuration["rounds"];
            }
        }
        else if (prefix == "winners") {
            auto suffix = key.substr(prefixPos);
            if (suffix.find("size")) {
                std::cout << "[CONTEXT] winers" << std::endl;
                return variables["winners"]; // TODO: as size
            }
        }
    
    // No prefix found; Check variables, constants, perPlayer, and perAudience
    } else {
        auto maps = {&variables, &constants, &perPlayer, &perAudience};
        for (const auto* map : maps) {
            if (auto it = map->find(key); it != map->end()) {
                return it->second;
            }
        }
    }

    std::cerr << "[CONTEXT] Could not find key: " << key << std::endl;
    return std::nullopt;
}

void GameContext::setVariable(std::string key, ExpressionVariant value) {
    if (auto it = variables.find(key); it != variables.end()) {
        it->second = value;
    } else {
        std::cerr << "[CONTEXT] Could not find variable: " << key << std::endl;
    }
}

std::unordered_map<std::string, ExpressionVariant> GameContext::getConfiguration() {
    return configuration;
}

std::unordered_map<std::string, ExpressionVariant> GameContext::getConstants() {
    return constants;
}

std::unordered_map<std::string, ExpressionVariant> GameContext::getVariables() {
    return variables;
}

std::unordered_map<std::string, ExpressionVariant> GameContext::getPerPlayer() {
    return perPlayer;
}

std::unordered_map<std::string, ExpressionVariant> GameContext::getPerAudience() {
    return perAudience;
}

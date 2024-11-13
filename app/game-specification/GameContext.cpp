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

    // Check for prefix
    auto prefixPos = key.find(".");
    if (prefixPos != std::string::npos) {
        // Has prefix
        auto prefix = key.substr(0, prefixPos);

        if (prefix == "configuration") {
            auto suffix = key.substr(prefixPos);
            if (suffix.find("rounds")) {
                return configuration["rounds"];
            }
        }
    
    // No prefix found; belongs to variables, constants, etc
    } else {
        if (auto it = variables.find(key); it != variables.end()) {
            return it->second;
        } else if (auto it = constants.find(key); it != constants.end()) {
            return it->second;
        }
    }
    return std::nullopt;
}

void GameContext::setVariable(std::string key, ExpressionVariant value) {
    if (auto it = variables.find(key); it != variables.end()) {
        it->second = value;
    } else {
        std::cout << "[CONTEXT] Could not find variable: " << key << std::endl;
    }
}
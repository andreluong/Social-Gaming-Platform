#include "GameContext.h"
#include "Utility.h"
#include <iostream>
#include <algorithm>
#include "BuiltInUtility.h"
#include <spdlog/spdlog.h>

GameContext::GameContext(std::shared_ptr<ExpressionMap> configuration, 
                        std::shared_ptr<ExpressionMap> constants,
                        std::shared_ptr<ExpressionMap> variables,
                        std::shared_ptr<ExpressionMap> perPlayer,
                        std::shared_ptr<ExpressionMap> perAudience,
                        std::shared_ptr<ExpressionMap> players)
    : configuration(std::move(configuration)),
    constants(std::move(constants)), 
    variables(std::move(variables)), 
    perPlayer(std::move(perPlayer)), 
    perAudience(std::move(perAudience)),
    players(std::move(players)) {}

// Check in variables, constants, perPlayer, perAudience, and players
std::optional<ExpressionWrapper> GameContext::findInExpressionMaps(const std::string_view& key) {
    auto maps = {variables, constants, perPlayer, perAudience, players};
    for (const auto map : maps) {
        if (auto it = map->find(std::string(key)); it != map->end()) {
            return it->second;
        }
    }
    spdlog::warn("[CONTEXT] Could not find key in any expression maps: {}", key);
    return std::nullopt;
}

// TODO: Does too much, we need additional functions to handle expressions (&&, ||, etc)
// Finds a value from all maps using a given key
// If the key contains a prefix, return specific variables from specific maps
std::optional<ExpressionWrapper> GameContext::find(const std::string_view& key) {
    // NOTE:
    // This uses a split object / function string design
    // We find twice: object and function
    // Object will return the value or size of the object
    // Upfrom will return the value inside
    if (key.find("upfrom") != std::string_view::npos) {
        auto argument = utility::extract(key);
        if (argument && utility::isNumber(argument.value())) {
            return ExpressionWrapper{std::stoi(std::string(argument.value()))};
        }
        throw std::invalid_argument{"[find] Invalid argument found for upfrom function. Upfrom should have a number argument."};
    }

    // NOTE:
    // This is one whole line passed in
    // It is badly coded to return whatever is necessary from analyzing the line
    if (auto builtInPair = builtin::splitFunction(key)) {
        // auto object = this->find(builtInPair->first);
        auto object = builtInPair->first;
        auto function = builtInPair->second;

        // Size
        if (function.find("size") != std::string_view::npos) {
            auto objectVariant = this->find(object);
            auto size = std::visit<int>(VisitSize{}, objectVariant->value);
            return ExpressionWrapper{size};
        }

        // Contains
        {
            // Check argument for object
            // Return boolean
        }

        return std::nullopt;
    }

    // Splits the key into a list delimited by a period
    // If found prefix, belongs to configuration, players, player, winners, weapons, or weapon
    auto keyList = utility::splitString(key);
    switch (keyList.size()) {
        case 1: 
            return findInExpressionMaps(key);

        // Specific map
        case 2: {
            auto prefix = keyList.at(0);
            auto suffix = keyList.at(1);
            
            if (prefix == "configuration") {
                if (suffix.find("rounds") != std::string_view::npos) {
                    auto it = configuration->find("rounds");
                    if (it != configuration->end()) {
                        return it->second;
                    }
                }
            }
            else if (prefix == "winners") {
                return ExpressionWrapper{(*variables)["winners"]};
            }
            break;
        }

        // Contains elements keyword
        case 3: {
            auto prefix = keyList.at(0);
            auto middle = keyList.at(1);
            auto suffix = keyList.at(2);
            
            break;
        }

        // Unknown
        default: {
            std::cout << "Default: " << std::endl;
            for (auto x : keyList) {
                std::cout << x << std::endl;
            }
            break;
        }
    }
    spdlog::warn("[CONTEXT] Could not find key: {}", key);
    return std::nullopt;
}

void GameContext::setVariable(std::string key, ExpressionWrapper value) {
    if (auto it = variables->find(key); it != variables->end()) {
        it->second = value;
    } else {
        spdlog::warn("[CONTEXT] Could not find variable: {}", key);
    }
}

std::shared_ptr<ExpressionMap> GameContext::getConfiguration() {
    return configuration;
}

std::shared_ptr<ExpressionMap> GameContext::getConstants() {
    return constants;
}

std::shared_ptr<ExpressionMap> GameContext::getVariables() {
    return variables;
}

std::shared_ptr<ExpressionMap> GameContext::getPerPlayer() {
    return perPlayer;
}

std::shared_ptr<ExpressionMap> GameContext::getPerAudience() {
    return perAudience;
}

std::shared_ptr<ExpressionMap> GameContext::getPlayers() {
    return players;
}

#include "GameContext.h"
#include "Utility.h"
#include <iostream>
#include <algorithm>
#include "BuiltInUtility.h"

GameContext::GameContext(std::shared_ptr<ExpressionMap> configuration, 
                        std::shared_ptr<ExpressionMap> constants,
                        std::shared_ptr<ExpressionMap> variables,
                        std::shared_ptr<ExpressionMap> perPlayer,
                        std::shared_ptr<ExpressionMap> perAudience,
                        std::vector<User> players)
    : configuration(std::move(configuration)),
    constants(std::move(constants)), 
    variables(std::move(variables)), 
    perPlayer(std::move(perPlayer)), 
    perAudience(std::move(perAudience)),
    players(players) {}

// Finds a value from all maps using a given key
// If the key contains a prefix, return specific variables from specific maps
std::optional<ExpressionWrapper> GameContext::find(std::string_view key) {
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
        }

        return std::nullopt;
    }

    // If found prefix, belongs to configuration, players, player, winners, weapons, or weapon
    auto keyList = utility::splitString(key);
    switch (keyList.size()) {
        // Check expression maps (variables, constants, perPlayer, and perAudience)
        case 1: {
            auto maps = {variables, constants, perPlayer, perAudience};
            for (const auto map : maps) {
                if (auto it = map->find(std::string(key)); it != map->end()) {
                    return it->second;
                }
            }
            break;
        }

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
            else if (prefix == "players") {
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
            std::cout << "default" << std::endl;
            for (auto x : keyList) {
                std::cout << x << std::endl;
            }
            break;
        }
    }

    std::cerr << "[CONTEXT] Could not find key: " << key << std::endl;
    return std::nullopt;
}

void GameContext::setVariable(std::string key, ExpressionWrapper value) {
    if (auto it = variables->find(key); it != variables->end()) {
        it->second = value;
    } else {
        std::cerr << "[CONTEXT] Could not find variable: " << key << std::endl;
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

std::vector<User> GameContext::getPlayers() {
    return players;
}

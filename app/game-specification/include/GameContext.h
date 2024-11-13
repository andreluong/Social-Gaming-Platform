#pragma once

#include <unordered_map>
#include <optional>
#include <string>
#include <dataVariant.h>

class GameContext {
public:
    std::unordered_map<std::string, ExpressionVariant> configuration;
    std::unordered_map<std::string, ExpressionVariant> constants;
    std::unordered_map<std::string, ExpressionVariant> variables;
    std::unordered_map<std::string, ExpressionVariant> perPlayer;
    std::unordered_map<std::string, ExpressionVariant> perAudience;

    GameContext(std::unordered_map<std::string, ExpressionVariant> configuration,
                std::unordered_map<std::string, ExpressionVariant> constants,
                std::unordered_map<std::string, ExpressionVariant> variables,
                std::unordered_map<std::string, ExpressionVariant> perPlayer,
                std::unordered_map<std::string, ExpressionVariant> perAudience);

    std::optional<ExpressionVariant> find(std::string key);

    void setVariable(std::string key, ExpressionVariant value);
};
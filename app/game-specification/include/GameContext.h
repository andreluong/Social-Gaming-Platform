#pragma once

#include <unordered_map>
#include <optional>
#include <string>
#include "dataVariant.h"

class GameContext {
public:
    GameContext(std::unordered_map<std::string, ExpressionVariant> configuration,
                std::unordered_map<std::string, ExpressionVariant> constants,
                std::unordered_map<std::string, ExpressionVariant> variables,
                std::unordered_map<std::string, ExpressionVariant> perPlayer,
                std::unordered_map<std::string, ExpressionVariant> perAudience);

    std::optional<ExpressionVariant> find(std::string key);

    void setVariable(std::string key, ExpressionVariant value);

    std::unordered_map<std::string, ExpressionVariant> getConfiguration();
    std::unordered_map<std::string, ExpressionVariant> getConstants();
    std::unordered_map<std::string, ExpressionVariant> getVariables();
    std::unordered_map<std::string, ExpressionVariant> getPerPlayer();
    std::unordered_map<std::string, ExpressionVariant> getPerAudience();

private:
    std::unordered_map<std::string, ExpressionVariant> configuration;
    std::unordered_map<std::string, ExpressionVariant> constants;
    std::unordered_map<std::string, ExpressionVariant> variables;
    std::unordered_map<std::string, ExpressionVariant> perPlayer;
    std::unordered_map<std::string, ExpressionVariant> perAudience;
};
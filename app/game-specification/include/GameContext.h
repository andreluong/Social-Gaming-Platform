#pragma once

#include <unordered_map>
#include <optional>
#include <string>
#include "dataVariant.h"
#include <string_view>
#include <vector>
#include "User.h"
#include <memory>

class GameContext {
public:
    GameContext(std::shared_ptr<ExpressionMap> configuration,
                std::shared_ptr<ExpressionMap> constants,
                std::shared_ptr<ExpressionMap> variables,
                std::shared_ptr<ExpressionMap> perPlayer,
                std::shared_ptr<ExpressionMap> perAudience,
                std::shared_ptr<ExpressionMap> players);

    std::optional<ExpressionWrapper> find(const std::string_view& key);

    void setVariable(std::string key, ExpressionWrapper value);

    std::shared_ptr<ExpressionMap> getConfiguration();
    std::shared_ptr<ExpressionMap> getConstants();
    std::shared_ptr<ExpressionMap> getVariables();
    std::shared_ptr<ExpressionMap> getPerPlayer();
    std::shared_ptr<ExpressionMap> getPerAudience();
    std::shared_ptr<ExpressionMap> getPlayers();

private:
    std::shared_ptr<ExpressionMap> configuration;
    std::shared_ptr<ExpressionMap> constants;
    std::shared_ptr<ExpressionMap> variables;
    std::shared_ptr<ExpressionMap> perPlayer;
    std::shared_ptr<ExpressionMap> perAudience;
    std::shared_ptr<ExpressionMap> players;

    std::optional<ExpressionWrapper> findInExpressionMaps(const std::string_view& key);
};
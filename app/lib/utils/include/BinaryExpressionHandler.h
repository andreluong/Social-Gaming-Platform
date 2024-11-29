#pragma once

#include <unordered_set>
#include <string>
#include <optional>
#include <vector>
#include "dataVariant.h"
#include "Utility.h"

namespace binaryExpression {

static std::unordered_set<std::string> comparisonOperators = {
    "=", "!=", "<", "<=", ">", ">="
};

static std::unordered_set<std::string> logicalOperators = {
    "||", "&&", "!"
};

static std::unordered_set<std::string> arithmeticOperators = {
    "+", "-", "*", "/", "%"
};

std::optional<std::string> findOperator(const std::string& s, const int& index);

bool evaluateBinaryExpression(std::vector<ExpressionVariant> vector);

}

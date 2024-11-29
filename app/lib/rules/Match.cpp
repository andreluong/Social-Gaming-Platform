#include "Rule.h"
#include "BuiltInUtility.h"
#include "Utility.h"
#include "BinaryExpressionHandler.h"

// Splits a binary expression into operands and operators, storing them in a vector
// If an operand is found, find the object in GameContext and store that
std::vector<ExpressionVariant> splitBinaryExpression(const std::string& s, GameContext* context) {
    std::vector<ExpressionVariant> result;

    // Split by operators
    auto start = 0;
    for (auto i = 0; i < s.size(); i++) {
        auto operatorIt = binaryExpression::findOperator(s, i);

        // Add operand and the operator to result
        if (operatorIt.has_value()) {
            auto foundOp = *operatorIt;
            if (start < i) {
                auto operand = s.substr(start, i - start);
                auto trimmedOperand = utility::trim(std::string(operand));
                if (utility::isNumber(trimmedOperand)) {
                    result.push_back(std::stoi(trimmedOperand));
                } else {
                    auto foundObject = context->find(trimmedOperand);
                    if (foundObject) {
                        result.push_back(foundObject->value);
                    }
                }
            }
            result.push_back(foundOp);
            i += foundOp.size() - 1;
            start = i + 1;
        }
    }

    // Add remaining string
    if (start < s.size()) {
        auto operand = s.substr(start);
        auto trimmedOperand = utility::trim(std::string(operand));
        if (utility::isNumber(trimmedOperand)) {
            result.push_back(std::stoi(trimmedOperand));
        } else {
            auto foundObject = context->find(trimmedOperand);
            if (foundObject) {
                result.push_back(foundObject->value);
            }
        }
    }

    return result;
}

Match::Match(std::string_view target, std::vector<std::unique_ptr<MatchEntry>> entries)
    : target(target), entries(std::move(entries)) {}

void Match::execute(GameContext* context) {
    spdlog::info("[Match] executing...");

    if (auto builtInPair = builtin::splitFunction(target); builtInPair.has_value()) {
        auto object = builtInPair->first;
        auto function = builtInPair->second;
        auto result = true; // default initialization

        // Check for negation !
        if (object.find('!') != std::string_view::npos) {
            result = false;
            spdlog::info("[Match] executing negated object: {}; function: {}", object, function);
        } else {
            spdlog::info("[Match] executing for object: {}; function: {}", object, function);
        }
        
    } else {
        // Check if target is a boolean
        // Assume match entry guards will be boolean expressions (only 2)
        if (utility::isBoolean(target)) {
            auto targetValue = utility::evaluateBoolean(target);

            // Check all entries in order for matching target
            // Execute the first one that matches
            for (const auto& entryPtr : entries) {

                auto guard = entryPtr->getGuard();
                spdlog::info("[Match] guard: {}", guard);
                std::optional<bool> result;
                if (utility::isBoolean(guard)) {
                    result = utility::evaluateBoolean(guard);
                } else {
                    auto evaluation = splitBinaryExpression(std::string{guard}, context);
                    result = binaryExpression::evaluateBinaryExpression(evaluation);
                }

                spdlog::info("[Match] result: {}; target: {}", result.value_or("null"), target);

                if (result == *targetValue) {
                    entryPtr->execute(context);
                    break;
                }
            }
        
        } else {
            auto contextTarget = context->find(target);
        }
    }
}

void Match::print() {
    std::cout << "Match: \nguard: " << target << "\nentries: \n";
    for (const auto& entryPtr : entries) {
        entryPtr->print();
    }
    std::cout << std::endl;
}

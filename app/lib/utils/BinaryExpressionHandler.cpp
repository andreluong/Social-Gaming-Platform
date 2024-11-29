#include "BinaryExpressionHandler.h"
#include <spdlog/spdlog.h>

namespace binaryExpression {

    // Finds and returns an operator if it belongs in the comparison, logical, and arithimetic operator sets
    std::optional<std::string> findOperator(const std::string& s, const int& index) {
        auto findOperatorInList = [&s, index](const auto& operators) {
            return std::find_if(operators.begin(), operators.end(), [index, s](auto op) {
                return s.substr(index, op.size()) == op;
            });
        };

        // Find comparison operators
        auto comparisonIt = findOperatorInList(comparisonOperators);
        if (comparisonIt != comparisonOperators.end()) {
            return *comparisonIt;
        }

        // Find logical operators
        auto logicalIt = findOperatorInList(logicalOperators);
        if (logicalIt != logicalOperators.end()) {
            return *logicalIt;
        }

        // Find arithmetic operators
        auto arithmeticIt = findOperatorInList(arithmeticOperators);
        if (arithmeticIt != arithmeticOperators.end()) {
            return *arithmeticIt;
        }

        return std::nullopt;
    }

    /*
        Private function:
        Evaluate the logical vector.
        Vector should only contain booleans (operands) and strings (operators).
    */  
    std::optional<bool> evaluateLogicalVector(std::vector<std::string> logicalVector) {
        std::optional<bool> result;

        for (auto i = 0; i < logicalVector.size()-1; i++) {
            auto cur = logicalVector.at(i);
            if (binaryExpression::logicalOperators.contains(cur)) {

                // If
                if (cur == "||") {
                    auto right = utility::evaluateBoolean(logicalVector.at(i+1));

                    if (!result.has_value()) {
                        auto left = utility::evaluateBoolean(logicalVector.at(i-1));
                        result = left || right;
                    } else {
                        result = result || right;
                    }
                    i++;
                }

                // And
                else if (cur == "&&") {
                    auto right = utility::evaluateBoolean(logicalVector.at(i+1));

                    if (!result.has_value()) {
                        auto left = utility::evaluateBoolean(logicalVector.at(i-1));
                        result = left && right;
                    } else {
                        result = result && right;
                    }
                    i++;
                }

                // Negation
                else {
                    if (!result.has_value()) {
                        auto right = utility::evaluateBoolean(logicalVector.at(i+1));
                        result = !right;
                        i++;
                    } else {
                        result = !result;
                    }
                }
            }
        }

        return result;
    }

    /*
        NOTE: some assumptions
        - no brackets
        - operands will only be ints
        - operators will only be strings

        TODO: Refactor to remove deep nesting and include more operators
    */
    bool evaluateBinaryExpression(std::vector<ExpressionVariant> vector) {
        // Holds the operand booleans and the logical operators to be evaluated
        std::vector<std::string> logicalVector; 

        if (vector.empty()) {
            return false;
        }

        /*
            Evalutes all the comparison operators and stores the result to logicalVector
            Any logical operators found, will also be stored in logicalVector

            TODO: Implement other operators
        */
        int left;
        for (auto i = 0; i < vector.size()-1; i++) {

            // Attempts to get the operator value first
            try {
                auto curOperator = std::visit(VisitString{}, vector.at(i));

                // Locates the correct operator
                if (binaryExpression::comparisonOperators.contains(curOperator)) {
                    if (curOperator == "=") {
                        auto right = std::visit(VisitInt{}, vector.at(i+1));
                        auto result = left == right;
                        spdlog::info("Evaluating: {} == {} = {}", left, right, result);
                        logicalVector.push_back(std::to_string(result));
                        i++;
                    }
                }
                else if (binaryExpression::logicalOperators.contains(curOperator)) {
                    logicalVector.push_back(curOperator);
                }

            // If failed to get the operator value, it is an operand
            // Assign the value to left
            } catch (const std::invalid_argument&) {
                left = std::visit(VisitInt{}, vector.at(i));
            }
        }
         
        std::optional<bool> result = evaluateLogicalVector(logicalVector);
        
        // Return result or default to false
        return result.has_value()
            ? result.value()
            : false;
    }
}
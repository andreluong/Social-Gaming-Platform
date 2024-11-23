#include "Rule.h"
#include "BuiltInUtility.h"
#include "Utility.h"
#include <unordered_set>

// TODO: Extract all the binary expression stuff into a class

std::unordered_set<std::string> comparisonOperators = {
    "=", "!=", "<", "<=", ">", ">="
};

std::unordered_set<std::string> logicalOperators = {
    "||", "&&", "!"
};

std::unordered_set<std::string> arithmeticOperators = {
    "+", "-", "*", "/", "%"
};

std::optional<std::string> findOperator(const std::string& s, const int& index) {
    auto comparisonIt = std::find_if(comparisonOperators.begin(), comparisonOperators.end(), [index, s](auto op) {
        return s.substr(index, op.size()) == op;
    });
    if (comparisonIt != comparisonOperators.end()) {
        return *comparisonIt;
    }

    auto logicalIt = std::find_if(logicalOperators.begin(), logicalOperators.end(), [index, s](auto op) {
        return s.substr(index, op.size()) == op;
    });
    if (logicalIt != logicalOperators.end()) {
        return *logicalIt;
    }

    auto arithmeticIt = std::find_if(arithmeticOperators.begin(), arithmeticOperators.end(), [index, s](auto op) {
        return s.substr(index, op.size()) == op;
    });
    if (arithmeticIt != arithmeticOperators.end()) {
        return *arithmeticIt;
    }

    return std::nullopt;
}


// Splits a binary expression into operands and operators, storing them in a vector
// If an operand is found, find the object in GameContext and store that
// TODO: Binary Expression Utility - Maybe move to different class
std::vector<ExpressionVariant> splitBinaryExpression(const std::string& s, GameContext* context) {
    std::vector<ExpressionVariant> result;

    // Split by operators
    auto start = 0;
    for (auto i = 0; i < s.size(); i++) {
        auto operatorIt = findOperator(s, i);

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

/*
    NOTE: some assumptions
    - no brackets
    - operands will only be ints
    - operators will only be strings

    TODO: Refactor to remove deep nesting and include more operators
*/
bool evaluateBinaryExpression(std::vector<ExpressionVariant> vector) {
    std::cout << "\n[MATCH] Evaluating..." << std::endl;

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
            if (comparisonOperators.contains(curOperator)) {
                if (curOperator == "=") {
                    auto right = std::visit(VisitInt{}, vector.at(i+1));
                    auto result = left == right;
                    std::cout << "[MATCH] " << left << " == " << right << " = " << result << std::endl;
                    logicalVector.push_back(std::to_string(result));
                    i++;
                }
            }
            else if (logicalOperators.contains(curOperator)) {
                std::cout << "operator: " << curOperator << std::endl;
                logicalVector.push_back(curOperator);
            }

        // If failed to get the operator value, it is an operand
        // Assign the value to left
        } catch (const std::invalid_argument&) {
            left = std::visit(VisitInt{}, vector.at(i));
        }
    }

    /*
        Evaluate the logical vector.
        Vector should only contain booleans (operands) and strings (operators).
    */    
    std::optional<bool> result;
    for (auto i = 0; i < logicalVector.size()-1; i++) {
        auto cur = logicalVector.at(i);
        if (logicalOperators.contains(cur)) {
            if (cur == "||") {
                auto right = utility::evaluateBoolean(logicalVector.at(i+1));

                if (!result.has_value()) {
                    auto left = utility::evaluateBoolean(logicalVector.at(i-1));
                    result = left || right;
                } else {
                    result = result || right;
                }
            }
        }
    }
    
    // Return result or default to false
    return result.has_value()
        ? result.value()
        : false;
}

Match::Match(std::string_view target, std::vector<std::unique_ptr<MatchEntry>> entries)
    : target(target), entries(std::move(entries)) {}

void Match::execute(GameContext* context) {
    std::cout << "Match execute" << std::endl;

    if (auto builtInPair = builtin::splitFunction(target); builtInPair.has_value()) {
        auto object = builtInPair->first;
        auto function = builtInPair->second;

        // Check for negation !
        
    } else {
        // Check if target is a boolean
        // Assume match entry guards will be boolean expressions (only 2)
        if (utility::isBoolean(target)) {
            auto targetValue = utility::evaluateBoolean(target);

            // Check all entries in order for matching target
            // Execute the first one that matches
            for (const auto& entryPtr : entries) {

                // TODO: Force to first entry for this specific scenario
                auto guard = entryPtr->getGuard();
                std::optional<bool> result;
                if (utility::isBoolean(guard)) {
                    result = utility::evaluateBoolean(guard);
                } else {
                    auto evaluation = splitBinaryExpression(std::string{guard}, context);
                    result = evaluateBinaryExpression(evaluation);
                    
                }

                std::cout << "result: " << *result << std::endl;
                std::cout << "target: " << *targetValue << std::endl;

                if (result == *targetValue) {
                    entryPtr->print();
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

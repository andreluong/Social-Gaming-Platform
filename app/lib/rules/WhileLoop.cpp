#include "Rule.h"
#include "Utility.h"

#include <iostream>
#include <stdexcept>

struct VisitWhileCondition {

    // Handles boolean condition
    bool operator()(bool value) const {
        return value;
    }

    // Handles integer condition, not 0 is considered as true
    bool operator()(int value) const {
        return value != 0;
    }

    template <typename T>
    bool operator()(T) const {
        throw std::invalid_argument{"[WhileLoop] Unsupported condition type"};
    }
};

WhileLoop::WhileLoop(std::string_view condition, std::vector<std::unique_ptr<Rule>> body)
    : condition(condition), body(std::move(body)) {}

void WhileLoop::execute(GameContext* context) {

    auto conditionWrapper = context->find(condition);

    if (!conditionWrapper) {
        std::cerr << "[WhileLoop] Condition not found in context: " << condition << std::endl;
        return;
    }

    while (true) {

        auto conditionValue = conditionWrapper->value;

        // Check if the current condition is valid
        if (!std::holds_alternative<bool>(conditionValue) &&
            !std::holds_alternative<int>(conditionValue)) {
            std::cerr << "[WhileLoop] Invalid type for condition in context: " << condition << std::endl;
            return;
        }

        // Check the condition
        bool conditionResult = std::visit(VisitWhileCondition{}, conditionValue);
        if (!conditionResult) {
            break;
        }

        // Execute statement list
        for (const auto& rulePointer : body) {
            rulePointer->execute(context);
        }

        // Update the condition
        conditionWrapper = context->find(condition);
        if (!conditionWrapper) {
            std::cerr << "[WhileLoop] Condition disappeared from context: " << condition << std::endl;
            return;
        }
    }
}


void WhileLoop::print() {
    std::cout << "While loop:\ncondition: " << condition << "\nbody: \n";
    for (const auto& rulePointer : body) {
        if (rulePointer) {
            rulePointer->print();
        }
    }
    std::cout << std::endl;
}

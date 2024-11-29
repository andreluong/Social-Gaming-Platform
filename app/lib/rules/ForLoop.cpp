#include "Rule.h"
#include "Utility.h"
#include "BuiltInUtility.h"
#include <string>

struct VisitForLoop {
    const std::string_view& key;
    std::vector<std::unique_ptr<Rule>>& rules;
    GameContext* context;

    void operator()(std::shared_ptr<ExpressionMap> map) {
        for (const auto& pair : *map) {
            for (const auto& rule : rules) {
                rule->freshVariables[key] = pair; 
                rule->execute(context);
            }
            std::cout << std::endl;
        }
    }

    void operator()(std::shared_ptr<ExpressionVector> vector) {
        for (const auto& element : *vector) {
            for (const auto& rule : rules) {
                rule->freshVariables[key] = element; 
                rule->execute(context);
            }
            std::cout << std::endl;
        }
    }

    template <typename T>
    void operator()(T) {
        throw std::invalid_argument{"[VisitForLoop] Invalid argument found"};
    }
};

ForLoop::ForLoop(std::string_view element, std::string_view list, std::vector<std::unique_ptr<Rule>> body)
    : element(element), list(list), body(std::move(body)) {}

void ForLoop::execute(GameContext* context) {
    spdlog::info("[For Loop] executing...");

    // Testing upfrom
    // TODO: If have time, do size. Size is not a priority
    if (auto builtInPair = builtin::splitFunction(list); builtInPair.has_value()) {
        auto object = builtInPair->first;
        auto function = builtInPair->second;

        auto maxWrapper = context->find(object);
        auto max = std::visit<int>(VisitInt{}, maxWrapper->value);
        auto minWrapper = context->find(function);
        auto min = std::visit<int>(VisitInt{}, minWrapper->value);

        for (int i = min; i <= max; i++) {
            spdlog::info("Iteration {}: ", i);
            for (const auto& rulePointer : body) {
                rulePointer->freshVariables[element] = i;    
                rulePointer->execute(context);
            }
            std::cout << std::endl;
        }
    }

    // Attempt to find a vector or map
    else if (auto contextList = context->find(list); contextList.has_value()) {
        auto listWrapper = contextList.value();
        std::visit<void>(VisitForLoop{element, body, context}, listWrapper.value);
    } 
    
    // No list found
    else {
        spdlog::error("[For Loop] no list found from context: {}", list);
    }
}

void ForLoop::print() {
    std::cout << "For loop:\nelement: " << element << "\nlist: " << list << "\nbody: \n";
    for (const auto& rulePointer : body) {
        if (rulePointer) {
            rulePointer->print();
        }
    }
    std::cout << std::endl;
}

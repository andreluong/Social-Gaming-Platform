#include "Rule.h"
#include "Utility.h"
#include "BuiltInUtility.h"

ForLoop::ForLoop(std::string_view element, std::string_view list, std::vector<std::unique_ptr<Rule>> body)
    : element(element), list(list), body(std::move(body)) {}

void ForLoop::execute(GameContext* context) {
    std::cout << "For loop execute" << std::endl;   

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
            std::cout << "\nIteration " << i << ":" << std::endl;
            for (const auto& rulePointer : body) {
                rulePointer->freshVariables[element] = i;    
                rulePointer->execute(context);
            }
            std::cout << std::endl;
        }
        return;
    }

    // Attempt to find a vector or map
    auto contextList = context->find(list);
    if (contextList) {
        auto wrapper = contextList.value();

        // Testing with a for-each loop using a list of values
        // Example: Weapons
        if (std::holds_alternative<std::shared_ptr<ExpressionVector>>(wrapper.value)) {
            std::cout << "[FOR LOOP] contains vector" << std::endl;

            auto listPointer = std::visit<std::shared_ptr<ExpressionVector>>(VisitReturnPointer{}, wrapper.value);
            auto& listValue = *listPointer;

            for (auto element : listValue) {
                for (const auto& rulePointer : body) {
                    rulePointer->execute(context);
                }
            }
            std::cout << std::endl;
        }
        
        else {
            std::cerr << "FOR LOOP: Unsupported type in context: " << list << std::endl;
        }
    } 

    // TODO: Handles the player list. Hardcoded for now
    // GameContext.find() returns an optional<ExpressionVariant>. Vector<User> does not belong
    // Find prints an error message and returns nullopt. Need to fix that
    else if (list == "players") {
        std::cout << "[FOR LOOP] Found players" << std::endl;
        for (auto player : context->getPlayers()) {
            std::cout << "Player " << player.getName() << std::endl;
        }
        std::cout << std::endl;
    }
    
    else {
        std::cerr << "[FOR LOOP] No list found from context: " << list << std::endl;
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

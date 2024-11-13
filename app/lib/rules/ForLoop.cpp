#include "Rule.h"

ForLoop::ForLoop(std::string_view element, std::string_view list, std::vector<std::unique_ptr<Rule>> body)
    : element(element), list(list), body(std::move(body)) {}

void ForLoop::execute(GameContext* context) {
    std::cout << "For loop execute" << std::endl;    
    
    auto contextList = context->find(std::string(list));
    if (contextList) {
        // Iterate for-loop with an integer
        if (std::holds_alternative<int>(contextList.value())) {
            auto listValue = std::visit<int>(VisitInt{}, contextList.value());

            for (int i = 0; i < listValue; i++) {
                std::cout << "Iteration " << i << ":" << std::endl;
                for (const auto& rulePointer : body) {
                    rulePointer->execute(context);
                }
                std::cout << std::endl;
            }
        } else {
            std::cerr << "FOR LOOP: Unsupported type in context: " << list << std::endl;
        }
        // auto listValue = std::visit<int>(VisitInt(), contextList.value());
    } else {
        std::cerr << "FOR LOOP: No list found from context: " << list << std::endl;
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

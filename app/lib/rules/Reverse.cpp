#include "Rule.h"

Reverse::Reverse(std::string_view target) 
    : target(target) {}

void Reverse::execute(GameContext* context) {

    std::cout << "[Reverse] Execute" << std::endl;   

    auto contextTarget = context->find(target);

    if (!contextTarget || !std::holds_alternative<std::shared_ptr<ExpressionVector>>(contextTarget->value)) {
        std::cerr << "[Reverse] Could not find target list: " << target << std::endl;
        return;
    }

    auto targetList = std::get<std::shared_ptr<ExpressionVector>>(contextTarget->value);

    std::reverse(targetList->begin(), targetList->end());
}

void Reverse::print() {
    std::cout << "[Reverse] \ntarget: " << target << std::endl << std::endl;
}

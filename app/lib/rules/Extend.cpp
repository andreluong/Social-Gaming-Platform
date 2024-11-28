#include "Rule.h"

#include <iostream>
#include <stdexcept>

struct VisitExtend {
    void operator()(std::shared_ptr<ExpressionVector>& targetList, 
                    std::shared_ptr<ExpressionVector>& valueList) {

        // Validate input lists
        if (targetList == nullptr || valueList == nullptr) {
            std::cerr << "[VisitExtend] Invalid lists provided" << std::endl;
            return;
        }

        // Append the contents of valueList to targetList
        targetList->insert(targetList->end(), valueList->begin(), valueList->end());
    }

    void operator()(auto a, auto b) {
        throw std::invalid_argument{"[VisitExtend] Invalid argument found"};
    }
};

Extend::Extend(std::string_view target, std::string_view value) 
    : target(target), value(value) {}

void Extend::execute(GameContext* context) {

    std::cout << "[Extend] Execute" << std::endl;    

    // Retrieve the target list and value list from the context
    auto contextTarget = context->find(target);
    auto contextValue = context->find(value);

    // Validate the target and value lists
    if (!(contextTarget && contextValue)) {
        std::cerr << "[Extend] Cannot find target: " << target << " or value: " << value << std::endl;
        return;
    }

    std::visit<void>(VisitExtend{}, contextTarget->value, contextValue->value);
}

void Extend::print() {
    std::cout << "Extend:\ntarget: " << target << "\nvalue: " << value << std::endl << std::endl;
}

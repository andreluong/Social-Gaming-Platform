#include "Rule.h"

ForLoop::ForLoop(std::string_view element, std::string_view list, std::vector<std::unique_ptr<Rule>> body)
    : element(element), list(list), body(std::move(body)) {}

void ForLoop::execute() {
    std::cout << "For loop execute" << std::endl;    
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

#include "Rule.h"

WhileLoop::WhileLoop(std::string_view condition, std::vector<std::unique_ptr<Rule>> body)
    : condition(condition), body(std::move(body)) {}

void WhileLoop::execute() {
    std::cout << "While loop execute" << std::endl;    
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

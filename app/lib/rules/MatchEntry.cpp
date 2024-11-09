#include "Rule.h"

MatchEntry::MatchEntry(std::string_view guard, std::vector<std::unique_ptr<Rule>> body)
        : guard(guard), body(std::move(body)) {}

void MatchEntry::execute() {
    std::cout << "Match entry execute" << std::endl;    
}

void MatchEntry::print() {
    std::cout << "Match Entry:\nguard: " << guard << "\nbody: \n";
    for (const auto& rulePointer : body) {
        if (rulePointer) {
            rulePointer->print();
        }
    }
    std::cout << std::endl;
}

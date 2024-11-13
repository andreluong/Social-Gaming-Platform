#include "Rule.h"

Match::Match(std::string_view target, std::vector<std::unique_ptr<MatchEntry>> entries)
    : target(target), entries(std::move(entries)) {}

void Match::execute(GameContext* context) {
    std::cout << "Match execute" << std::endl;    
}

void Match::print() {
    std::cout << "Match: \nguard: " << target << "\nentries: \n";
    for (const auto& entryPtr : entries) {
        if (entryPtr) {
            entryPtr->print();
        }
    }
    std::cout << std::endl;
}

#include "Rule.h"
#include "BuiltInUtility.h"

Match::Match(std::string_view target, std::vector<std::unique_ptr<MatchEntry>> entries)
    : target(target), entries(std::move(entries)) {}

void Match::execute(GameContext* context) {
    std::cout << "Match execute" << std::endl;

    if (auto builtInPair = builtin::splitFunction(target); builtInPair.has_value()) {
        auto object = builtInPair->first;
        auto function = builtInPair->second;

        // Check for negation !
        

    } else {
        auto contextTarget = context->find(target);

    }
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

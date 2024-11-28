#include "Rule.h"

#include <iostream>
#include <stdexcept>

struct VisitDeal {

    void operator()(std::shared_ptr<ExpressionVector>& sourceList, 
                    std::shared_ptr<ExpressionVector>& targetList, 
                    int count) {

        auto originalSourceCount = sourceList->size();
        auto targetCount = targetList->size();

        if (count <= 0 || sourceList->empty() || targetCount == 0) {
            std::cerr << "[VisitDeal] Invalid operation: count = " << count 
                      << ", source size = " << originalSourceCount 
                      << ", target count = " << targetCount << std::endl;
            return;
        }

        size_t totalDeals = static_cast<size_t>(count);
        
        for (size_t i = 0; i < totalDeals && !sourceList->empty(); ++i) {

            auto item = sourceList->back();
            sourceList->pop_back();

            // Select the target to deal to by round-robin
            auto& target = (*targetList)[i % targetList->size()];

            // Deal from source to target
            if (auto targetVector = std::get_if<std::shared_ptr<ExpressionVector>>(&target.value)) {
                (*targetVector)->push_back(item);
            } else {
                std::cerr << "[VisitDeal] Target is not a valid ExpressionVector" << std::endl;
            }
        }

        std::cout << "[VisitDeal] Original Source Size: " << originalSourceCount 
                  << ", New Source Size: " << sourceList->size() << std::endl;
    }

    // Fallback
    void operator()(auto a, auto b, auto c) {
        throw std::invalid_argument{"[VisitDeal] Invalid argument types provided"};
    }
};

Deal::Deal(std::string_view count, std::string_view targets, std::string_view source) 
    : count(count), targets(targets), source(source) {}

void Deal::execute(GameContext* context) {
    std::cout << "[Deal] Execute" << std::endl;

    // Look up the count, targets, and source in the context
    auto contextCount = context->find(count);
    auto contextTargets = context->find(targets);
    auto contextSource = context->find(source);

    // Validate all inputs
    if (!(contextCount && contextTargets && contextSource)) {
        std::cerr << "[Deal] Unable to find one or more of count: " << count 
                  << ", targets: " << targets << ", source: " << source << std::endl;
        return;
    }

    // Use VisitDeal to perform deal
    std::visit<void>(VisitDeal{}, contextSource->value, contextTargets->value, contextCount->value);
}

void Deal::print() {
    std::cout << "[Deal] \ncount: " << count << "\ntargets: " << targets << "\nsource: " 
              << source << std::endl << std::endl;
}

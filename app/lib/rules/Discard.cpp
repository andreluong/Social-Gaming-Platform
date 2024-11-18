#include "Rule.h"

Discard::Discard(std::string_view count, std::string_view source) 
    : count(count), source(source) {}

void Discard::execute(GameContext* context) {
    std::cout << "[Discard] Execute" << std::endl;    

    auto contextCount = context->find(count);
    auto contextSource = context->find(source);

    // Validate count and source
    if (!(contextCount && contextSource)) {
        std::cerr << "[Discard] Can not find count: " << count << " or source: " << source << std::endl;
        return;
    }

    std::visit<void>(VisitDiscard{}, contextSource->value, contextCount->value);
}

void Discard::print() {
    std::cout << "[Discard] \ncount: " << count << "\nsource: " << source << std::endl << std::endl;
}

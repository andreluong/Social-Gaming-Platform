#include "Rule.h"

// TODO: Currently discards from a vector
struct VisitDiscard {
    void operator()(std::shared_ptr<ExpressionVector>& listPointer, int count) {
        auto originalSize = listPointer->size();
        if (count > 0) {
            if (static_cast<std::size_t>(count) >= listPointer->size()) {
                listPointer->clear();
            } else {
                listPointer->erase(listPointer->end() - count, listPointer->end());
            }
        }
        std::cout << "[VisitDiscard] Original Size: " << originalSize << "; New size: " << listPointer->size() << std::endl;
    }

    void operator()(auto T, auto K) {
        throw std::invalid_argument{"[VisitDiscard] Invalid argument found"};
    }
};

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

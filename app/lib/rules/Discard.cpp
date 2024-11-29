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
        spdlog::info("[Discard] original size: {}; new size: {}", originalSize, listPointer->size());
    }

    void operator()(auto T, auto K) {
        throw std::invalid_argument{"[VisitDiscard] Invalid argument found"};
    }
};

Discard::Discard(std::string_view count, std::string_view source) 
    : count(count), source(source) {}

void Discard::execute(GameContext* context) {
    spdlog::info("[Discard] executing...");

    auto contextCount = context->find(count);
    auto contextSource = context->find(source);

    // Validate count and source
    if (!(contextCount && contextSource)) {
        spdlog::error("[Discard] could not find count: {}, or source: {}", count, source);
        return;
    }

    std::visit<void>(VisitDiscard{}, contextSource->value, contextCount->value);
}

void Discard::print() {
    std::cout << "[Discard] \ncount: " << count << "\nsource: " << source << std::endl << std::endl;
}

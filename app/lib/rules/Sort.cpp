#include "Rule.h"

#include <iostream>

Sort::Sort(std::string_view listName, std::optional<std::string_view> key)
    : listName(listName), key(key ? std::string(key.value()) : std::nullopt) {}

void Sort::execute(GameContext* context) {

    auto listWrapper = context->find(listName);

    if (!listWrapper) {
        std::cerr << "[Sort] List not found in context: " << listName << std::endl;
        return;
    }

    auto& listVariant = listWrapper->value;

    // Check that the list is of type ExpressionVector
    if (!std::holds_alternative<std::shared_ptr<ExpressionVector>>(listVariant)) {
        std::cerr << "[Sort] Target is not a list: " << listName << std::endl;
        return;
    }

    auto list = std::get<std::shared_ptr<ExpressionVector>>(listVariant);
    if (!list || list->empty()) {
        std::cerr << "[Sort] List is null or empty: " << listName << std::endl;
        return;
    }

    // Sort by optional key
    if (!key) {
        // Sort the list elements directly
        std::sort(list->begin(), list->end(), [](const ExpressionWrapper& a, const ExpressionWrapper& b) {
            return a.value < b.value;
        });
    } else {
        // Sort the list elements based on the provided key
        std::sort(list->begin(), list->end(), [&](const ExpressionWrapper& a, const ExpressionWrapper& b) {
            if (!std::holds_alternative<std::shared_ptr<ExpressionMap>>(a.value) ||
                !std::holds_alternative<std::shared_ptr<ExpressionMap>>(b.value)) {
                std::cerr << "[Sort] Elements in the list are not maps" << std::endl;
                return false;
            }

            auto mapA = std::get<std::shared_ptr<ExpressionMap>>(a.value);
            auto mapB = std::get<std::shared_ptr<ExpressionMap>>(b.value);

            if (!mapA->contains(*key) || !mapB->contains(*key)) {
                std::cerr << "[Sort] Key not found in one or more maps: " << *key << std::endl;
                return false;
            }

            return mapA->at(*key).value < mapB->at(*key).value;
        });
    }
}

void Sort::print() const {
    std::cout << "Sort operation:\nList: " << listName;
    if (key) {
        std::cout << "\nKey: " << *key;
    }
    std::cout << std::endl;
}

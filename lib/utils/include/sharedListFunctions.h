#ifndef SHARED_LIST_FUNCTIONS_H
#define SHARED_LIST_FUNCTIONS_H

#include <vector>
#include <functional>
#include <stdexcept>

// Uses static methods instead of composition because they are simple operations that are not expected to change
template <typename T>
class SharedListFunctions {
public:
    static std::vector<T> slice(const std::vector<T>& elements, size_t start, size_t end);
    static bool contains(const std::vector<T>& elements, const T& element);

    // Return a filtered list
    static std::vector<T> collect(const std::vector<T>& elements, std::function<bool(const T&)> predicate);
};

#include "../sharedListFunctions.tpp"

#endif

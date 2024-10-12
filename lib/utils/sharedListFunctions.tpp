#include "include/sharedListFunctions.h"

template <typename T>
std::vector<T> SharedListFunctions<T>::slice(const std::vector<T>& elements, size_t start, size_t end) {

    if (start > end || end > elements.size()) {
        throw std::out_of_range("Invalid slice range");
    }

    return std::vector<T>(elements.begin() + start, elements.begin() + end);
}

template <typename T>
bool SharedListFunctions<T>::contains(const std::vector<T>& elements, const T& element) {

    return std::find(elements.begin(), elements.end(), element) != elements.end();
}

template <typename T>
std::vector<T> SharedListFunctions<T>::collect(const std::vector<T>& elements, std::function<bool(const T&)> predicate) {

    std::vector<T> result;

    for (const auto& element : elements) {

        if (predicate(element)) {
            result.push_back(element);
        }
    }

    return result;
}

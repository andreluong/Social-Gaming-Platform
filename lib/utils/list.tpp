#include "include/list.h"

template <typename T>
List<T>::List(const std::vector<T>& elements) : elements(elements) {}

template <typename T>
const T& List<T>::at(size_t index) const {

    if (index >= elements.size()) {
        throw std::out_of_range("List: Index out of range");
    }

    return elements.at(index);
}

template <typename T>
List<T> List<T>::slice(size_t start, size_t end) const {

    std::vector<T> sliced = SharedListFunctions<T>::slice(elements, start, end);
    return List<T>(sliced);
}

template <typename T>
bool List<T>::contains(const T& element) const {
    return SharedListFunctions<T>::contains(elements, element);
}

template <typename T>
List<T> List<T>::collect(std::function<bool(const T&)> predicate) const {

    std::vector<T> collected = SharedListFunctions<T>::collect(elements, predicate);
    return List<T>(collected);
}


#ifndef LIST_H
#define LIST_H

#include <vector>
#include <stdexcept>
#include "sharedListFunctions.h"

template <typename T>
class List {
public:
    List(const std::vector<T>& elements);

    size_t size() const { return elements.size(); }
    const T& at(size_t index) const;
    List<T> slice(size_t start, size_t end) const;
    bool contains(const T& element) const;
    List<T> collect(std::function<bool(const T&)> predicate) const;

private:
    std::vector<T> elements;
};

#include "../list.tpp"

#endif

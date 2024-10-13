#ifndef MAPLIST_H
#define MAPLIST_H

#include <vector>
#include <map>
#include <functional>
#include <stdexcept>
#include "list.h"
#include "sharedListFunctions.h"

template <typename K, typename V>
class MapList {
public:
    MapList(const std::vector<std::map<K, V>>& elements);

    size_t size() const { return elements.size(); }
    const std::map<K, V>& at(size_t index) const;
    List<std::map<K, V>> slice(size_t start, size_t end) const;
    bool contains(const std::map<K, V>& map) const;
    List<std::map<K, V>> collect(std::function<bool(const std::map<K, V>&)> predicate) const;

    // Get values by key
    List<V> getValues(const K& key) const; 

private:
    std::vector<std::map<K, V>> elements;
};

#include "../mapList.tpp"

#endif

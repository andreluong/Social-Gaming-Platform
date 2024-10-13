#include "include/mapList.h"

template <typename K, typename V>
MapList<K, V>::MapList(const std::vector<std::map<K, V>>& elements) : elements(elements) {}

// Return the map at the specified index
template <typename K, typename V>
const std::map<K, V>& MapList<K, V>::at(size_t index) const {

    if (index >= elements.size()) {
        throw std::out_of_range("mapList: Index out of range");
    }

    return elements.at(index); 
}

template <typename K, typename V>
List<std::map<K, V>> MapList<K, V>::slice(size_t start, size_t end) const {

    std::vector<std::map<K, V>> sliced = SharedListFunctions<std::map<K, V>>::slice(elements, start, end);
    return List<std::map<K, V>>(sliced);
}

template <typename K, typename V>
bool MapList<K, V>::contains(const std::map<K, V>& map) const {
    return SharedListFunctions<std::map<K, V>>::contains(elements, map);
}

template <typename K, typename V>
List<std::map<K, V>> MapList<K, V>::collect(std::function<bool(const std::map<K, V>&)> predicate) const {

    std::vector<std::map<K, V>> collected = SharedListFunctions<std::map<K, V>>::collect(elements, predicate);
    return List<std::map<K, V>>(collected);
}

template <typename K, typename V>
List<V> MapList<K, V>::getValues(const K& key) const {

    std::vector<V> values;

    for (const auto& map : elements) { 

        if (map.find(key) != map.end()) {
            // Add the value associated with the key
            values.push_back(map.at(key));
        }
    }

    return List<V>(values);
}
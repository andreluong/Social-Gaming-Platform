#include "ValueMap.h"
#include "Constants.h"
#include <iostream>


std::unordered_map<std::string, ValueType> ValueMap::getValues() const {
    return values;
}

// Get a constant value by name (key)
std::optional<ValueType> ValueMap::getValue(const std::string& name) const {
    auto it = values.find(name);
    if (it != values.end()) {
        return it->second;
    }

    // Not found
    return std::nullopt;
}

// Print all values
void ValueMap::printValues() const {
    // only print keys for now - need to provide print override information for value variant
    for (const auto& [key, value] : values) {
        std::cout << key << std::endl;
    }
}

// Set an existing value by name (key)
void ValueMap::setValue(const std::string& name, const ValueType& value) {
    values[name] = value;
}

#include "Constants.h"
#include <iostream>


std::unordered_map<std::string, ValueType> Constants::getConstants() const {
    return constants;
}

// Get a constant value by name (key)
std::optional<ValueType> Constants::getConstant(const std::string& name) const {
    auto it = constants.find(name);
    if (it != constants.end()) {
        return it->second;
    }

    // Not found
    return std::nullopt;
}

// Print all constants
void Constants::printConstants() const {
    //
}

// Add a new constant to the map
void Constants::addConstant(const std::string& name, const ValueType& value) {
    constants[name] = value;
}

// Set an existing constant's value by name (key)
void Constants::setConstant(const std::string& name, const ValueType& value) {
    constants[name] = value;
}

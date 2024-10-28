#ifndef VALUEMAP_H
#define VALUEMAP_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <optional>

// Variant to-be updated
using Variant = std::variant<std::string, int, bool>;
using ValueType = std::variant<std::string, int, bool, std::unordered_map<std::string, Variant>, std::vector<Variant>>;

class GameSpecificationFactory;

class ValueMap {
public:
    ValueMap() = default;
    ValueMap(std::unordered_map<std::string, ValueType> values) : values(values) {}

    std::unordered_map<std::string, ValueType> getValues() const;
    std::optional<ValueType> getValue(const std::string& name) const;
    void printValues() const;

private:
    std::unordered_map<std::string, ValueType> values;

    // Private setters accessible only by friend class
    void setValue(const std::string& name, const ValueType& value);

    // Only friend class should access setters
    friend class GameSpecificationFactory;
};

#endif

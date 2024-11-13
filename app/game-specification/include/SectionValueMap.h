#ifndef VALUEMAP_H
#define VALUEMAP_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <optional>
#include "dataVariant.h"

// Variant to-be updated
// TODO: Use ExpressionVariant instead
using Variant = std::variant<std::string, int, bool>;
using ValueType = std::variant<std::string, int, bool, std::unordered_map<std::string, Variant>, std::vector<Variant>>;

class GameSpecificationParser;

class ValueMap {
public:
    ValueMap() = default;
    ValueMap(std::unordered_map<std::string, ExpressionVariant> values) : values(values) {}

    std::unordered_map<std::string, ExpressionVariant> getValues() const;
    std::optional<ExpressionVariant> getValue(const std::string& name) const;
    void printValues() const;

private:
    std::unordered_map<std::string, ExpressionVariant> values;

    // Private setters accessible only by friend class
    void setValue(const std::string& name, const ExpressionVariant& value);

    // Only friend class should access setters
    friend class GameSpecificationParser;
};

#endif

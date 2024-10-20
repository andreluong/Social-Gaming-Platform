#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <optional>

// Not sure
using Variant = std::variant<std::string, int, bool>;
using ValueType = std::variant<std::string, int, bool, std::unordered_map<std::string, Variant>, std::vector<Variant>>;

class GameSpecificationFactory;

class Constants {
public:
    Constants() = default;

    std::unordered_map<std::string, ValueType> getConstants() const;
    std::optional<ValueType> getConstant(const std::string& name) const;
    void printConstants() const;

private:
    std::unordered_map<std::string, ValueType> constants;

    // Private setters accessible only by friend class
    void addConstant(const std::string& name, const ValueType& value);
    void setConstant(const std::string& name, const ValueType& value);

    // Only friend class should access setters
    friend class GameSpecificationFactory;
};

#endif
#include "SectionValueMap.h"
#include "Constants.h"
#include <iostream>


ExpressionMap ValueMap::getValues() const {
    return values;
}

// Get a constant value by name (key)
std::optional<ExpressionVariant> ValueMap::getValue(const std::string& name) const {
    auto it = values.find(name);
    if (it != values.end()) {
        return it->second.value;
    }

    // Not found
    return std::nullopt;
}

// TODO: Commented out due to errors with changing to ExpressionVariant
// The variant is always string here - need to be fixed
void ValueMap::printValues() const {
    // for (const auto& [key, value] : values) {
    //     std::cout << key << ": ";
        
    //     std::visit([](const auto& val) {
    //         if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::string>) {
    //             std::cout << "String: " << val << std::endl;
    //         } else if constexpr (std::is_same_v<std::decay_t<decltype(val)>, int>) {
    //             std::cout << "Int: " << val << std::endl;
    //         } else if constexpr (std::is_same_v<std::decay_t<decltype(val)>, bool>) {
    //             std::cout << "Bool: " << (val ? "true" : "false") << std::endl;
    //         } else if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::unordered_map<std::string, Variant>>) {
    //             std::cout << "Map: {" << std::endl;
    //             for (const auto& [mapKey, mapValue] : val) {
    //                 std::cout << "  " << mapKey << ": ";
    //                 std::visit([](const auto& innerVal) {
    //                     std::cout << innerVal << std::endl;
    //                 }, mapValue);
    //             }
    //             std::cout << "}" << std::endl;
    //         } else if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::vector<Variant>>) {
    //             std::cout << "Vector: [";
    //             for (size_t i = 0; i < val.size(); ++i) {
    //                 std::visit([](const auto& innerVal) {
    //                     std::cout << innerVal << " ";
    //                 }, val[i]);
    //                 if (i != val.size() - 1) std::cout << ", ";
    //             }
    //             std::cout << "]" << std::endl;
    //         }
    //     }, value);
    // }
}

// Set an existing value by name (key)
void ValueMap::setValue(const std::string& name, const ExpressionVariant& value) {
    values[name] = ExpressionWrapper{value};
}

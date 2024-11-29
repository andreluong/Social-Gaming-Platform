#include "BuiltInUtility.h"
#include <iostream>

namespace builtin {
    // Splits the key into two. One contains the characters before the builtin function.
    // The other contains the builtin function and after.
    // If no builtin functions were found, return nullopt
    // e.g. upfrom, size, contains, and collect
    std::optional<std::pair<std::string_view, std::string_view>> splitFunction(const std::string_view& key) {
        std::vector<std::string_view> builtInFunctions = {
            ".upfrom(", ".size(", ".contains(", ".collect("
        };

        for (auto function : builtInFunctions) {
            auto beforeFunctionPos = key.find(function);
            if (beforeFunctionPos != std::string_view::npos) {
                auto beforeFunction = key.substr(0, beforeFunctionPos);
                auto afterFunction = key.substr(beforeFunctionPos + 1);
                return std::make_pair(beforeFunction, afterFunction);
            }
        }
        return std::nullopt;
    }
}
#pragma once

#include <vector>
#include <string_view>
#include <optional>
#include <string>

namespace utility {
    std::vector<std::string_view> splitString(const std::string_view& sv, const char& delimiter = '.');
    
    std::vector<int> stringViewToIntVector(const std::vector<std::string_view>& vector);

    std::optional<std::string_view> extract(const std::string_view& sv, 
                                                const char& openBracket = '(', 
                                                const char& closeBracket = ')');

    std::string toLowerCase(const std::string_view& sv);

    bool isNumber(const std::string_view& sv);

    bool isBoolean(const std::string_view& sv);

    std::optional<bool> evaluateBoolean(const std::string_view& sv);

    std::string trim(std::string s);
}
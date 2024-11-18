#pragma once

#include <vector>
#include <string_view>
#include <optional>

namespace utility {
    std::vector<std::string_view> splitString(const std::string_view& sv, const char& delimiter = '.');
    
    std::vector<int> stringViewToIntVector(const std::vector<std::string_view>& vector);

    std::optional<std::string_view> extract(const std::string_view& sv, 
                                                const char& openBracket = '(', 
                                                const char& closeBracket = ')');

    bool isNumber(const std::string_view& sv);
}
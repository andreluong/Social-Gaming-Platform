#include "Utility.h"
#include <algorithm>
#include <ranges>

namespace utility {
    // Returns a vector of splitted strings based on the delimiter
    std::vector<std::string_view> splitString(const std::string_view& sv, const char& delimiter) {
        auto splitView = sv
            | std::ranges::views::split(delimiter)
            | std::ranges::views::transform([](auto&& range) {
                const auto size = std::ranges::distance(range);
                return std::string_view(&*range.begin(), size);
            });
        
        return {splitView.begin(), splitView.end()};
    }

    std::vector<int> stringViewToIntVector(const std::vector<std::string_view>& vector) {
        std::vector<int> result;
        for (auto str : vector) {
            result.emplace_back(std::stoi(std::string(str)));
        }
        return result;
    }

    // Extract text within brackets
    std::optional<std::string_view> extract(const std::string_view& sv, 
                                                const char& openBracket, 
                                                const char& closeBracket) {
        auto openPos = sv.find(openBracket);
        auto closePos = sv.rfind(closeBracket);

        // Check the positions
        if (openPos == std::string_view::npos || 
            closePos == std::string_view::npos || 
            closePos <= openPos + 1) {
            return std::nullopt;
        }

        return sv.substr(openPos + 1, closePos - openPos - 1);
    }

    bool isNumber(const std::string_view& sv) {
        return !sv.empty() && std::all_of(sv.begin(), sv.end(), ::isdigit);
    }
}
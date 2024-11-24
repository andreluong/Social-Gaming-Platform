#include "Utility.h"
#include <algorithm>
#include <ranges>
#include <unordered_set>
#include <iostream>

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

    std::string toLowerCase(const std::string_view& sv) {
        std::string result{sv};
        std::transform(result.begin(), result.end(), result.begin(), [](const auto& c) {
            return std::tolower(c);
        });
        return result;
    }

    bool isNumber(const std::string_view& sv) {
        return !sv.empty() && std::all_of(sv.begin(), sv.end(), ::isdigit);
    }

    bool isBoolean(const std::string_view& sv) {
        if (sv.empty()) {
            return false;
        }
        
        std::unordered_set<std::string_view> booleans = {
            "true", "false", "0", "1"
        };

        return booleans.find(toLowerCase(sv)) != booleans.end();
    }

    std::optional<bool> evaluateBoolean(const std::string_view& sv) {
        if (!isBoolean(sv)) {
            std::cerr << "[evaluateBoolean] Input is not a valid boolean: " << sv << std::endl;
            return std::nullopt;
        }

        // Evaluate true values
        std::unordered_set<std::string_view> trueValues = {"true", "!false", "1"};
        if (trueValues.find(toLowerCase(sv)) != trueValues.end()) {
            return true;
        }

        // Evaluates false values
        std::unordered_set<std::string_view> falseValues = {"false", "!true", "0"};
        if (falseValues.find(toLowerCase(sv)) != falseValues.end()) {
            return false;
        }

        std::cerr << "[evaluateBoolean] Input is not a valid boolean." << std::endl;
        return std::nullopt;
    }

    std::string trim(std::string s) {
        auto notSpace = [](auto c) {
            return !std::isspace(c);
        };
        // Trim leading white space
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));

        // Trim trailing white space
        s.erase(std::find_if(s.rbegin(), s.rend(), notSpace).base(), s.end());
        
        return s;
    }
}
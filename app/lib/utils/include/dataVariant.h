#ifndef DATAVARIANT_H
#define DATAVARIANT_H

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <string_view>
#include <iostream>

using ExpressionVariant = std::variant<
    bool,
    int,
    float,
    std::string_view, // quoted_string, string_text, string_interpolation?
    std::vector<std::string_view>, // TODO: Should be of ExpressionVariant
    // Identifier    
    std::unordered_map<std::string, std::string> // TODO: Should be of ExpressionVariant
>;

// TODO: Remove this to use ExpressionVariant
using dataVariant = std::variant<
    int,
    std::string_view
    // ExpressionVariant
    // qualified identifier
    // body
    // comment
>;

struct VisitBoolean {
    void operator()(const bool& b) const;
};

struct VisitInt {
    int operator()(int value) const {
        return value;
    }
    
    int operator()(auto value) const{
        auto error = "[VARIANT] Invalid argument found for VisitInt";
        throw std::invalid_argument{error};
	}
};

struct VisitFloat {
    void operator()(const float& num) const;
};

struct VisitDouble {
    void operator()(const double& num) const;
};

struct VisitString {
    void operator()(const std::string_view& str) const {
        std::cout << "var from string_view: " << str << std::endl;
    }

    void operator()(const int& str) const {
        std::cout << "var from int: " << std::to_string(str) << std::endl;
    }

    void operator()(std::vector<std::string_view>& vec) const {
        for (auto e : vec) {
            std::cout << "var from string_view vector: " << e << std::endl;
        }
    }
};

struct VisitVector {
    template <typename T>
    std::vector<T> operator()(std::vector<T> value) {
        return value;
    }

    template <typename T>
    std::vector<T> operator()(T value) const{
        auto error = "[VARIANT] Invalid argument found for VisitVector";
        throw std::invalid_argument{error};
	}
};

#endif
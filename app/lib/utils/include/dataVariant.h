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
    double,
    std::string_view, // quoted_string, string_text, string_interpolation?
    std::vector<std::string_view> // TODO: Should be of ExpressionVariant
    // Identifier    
    // std::unordered_map<expressionVariant, expressionVariant>
>;

// TODO: May remove this to use ExpressionVariant
using dataVariant = std::variant<
    ExpressionVariant
    // qualified identifier
    // body
    // comment
>;

struct VisitBoolean {
    void operator()(const bool& b) const;
};

struct VisitInt {
    void operator()(const int& exp) const {
        std::cout << "var from int: " << exp << std::endl;
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

#endif
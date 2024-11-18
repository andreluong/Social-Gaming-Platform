#pragma once

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <optional>
#include "User.h"

struct ExpressionWrapper;

using ExpressionVector = std::vector<ExpressionWrapper>;
using ExpressionMap = std::unordered_map<std::string, ExpressionWrapper>;
using ExpressionVariant = std::variant<
    bool,
    int,
    float,
    std::string, // quoted_string, string_text, string_interpolation?
    std::shared_ptr<ExpressionVector>,
    // Identifier    
    std::shared_ptr<ExpressionMap>
>;

struct ExpressionWrapper {
    ExpressionVariant value;
};

// TODO: Currently hardcoded for the game file
using GameVariant = std::variant<
    int,                                        // round
    std::pair<std::string, ExpressionWrapper>,  // weapon
    User                                        // player
>;

// TODO: Remove this to use ExpressionVariant
using dataVariant = std::variant<
    int,
    std::string
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

    template <typename T>
    int operator()(T) const {
        throw std::invalid_argument{"[VisitInt] Invalid argument found"};
    }
};

struct VisitFloat {
    void operator()(const float& num) const;
};

struct VisitString {
    std::string operator()(std::string value) const {
        return value;
    }

    template <typename T>
    std::string operator()(T value) const{
        throw std::invalid_argument{"[VisitString] Invalid argument found"};
	}    
};

struct VisitVector {
    template <typename T>
    std::vector<T> operator()(std::vector<T> value) {
        return value;
    }

    template <typename T>
    std::vector<T> operator()(T value) const{
        throw std::invalid_argument{"[VisitVector] Invalid argument found"};
	}
};



// TODO: Currently prints the valeus in a map
// Used for checking weapons
struct VisitPrinter {
    void operator()(std::shared_ptr<ExpressionMap> map) {
        for (auto x : *map) {
            auto y = std::visit(VisitString{}, x.second.value);
            std::cout << "key: " << x.first << "; value: " << y << std::endl;
        }
    }
    
    template <typename T>
    void operator()(T value) const{
        throw std::invalid_argument{"[VisitHelp] Invalid argument found"};
	}
};

// TODO: Temporary. Returns the pointer of a vector. Can't also return a map
struct VisitReturnPointer {
    std::shared_ptr<ExpressionVector> operator()(std::shared_ptr<ExpressionVector> pointer) {
        return pointer;
    }

    template <typename T>
    std::shared_ptr<ExpressionVector> operator()(T value) const{
        throw std::invalid_argument{"[VisitReturnPointer] Invalid argument found"};
	}
};

// TODO: Currently discards from a vector
struct VisitDiscard {
    void operator()(std::shared_ptr<ExpressionVector>& listPointer, int count) {
        auto originalSize = listPointer->size();
        if (count > 0) {
            if (static_cast<std::size_t>(count) >= listPointer->size()) {
                listPointer->clear();
            } else {
                listPointer->erase(listPointer->end() - count, listPointer->end());
            }
        }
        std::cout << "[VisitDiscard] Original Size: " << originalSize << "; New size: " << listPointer->size() << std::endl;
    }

    void operator()(auto T, auto K) {
        throw std::invalid_argument{"[VisitDiscard] Invalid argument found"};
    }
};

struct VisitSize {
    std::size_t operator()(std::shared_ptr<ExpressionMap> map) const {
        return map ? map->size() : 0;
    }

    std::size_t operator()(std::shared_ptr<ExpressionVector> vec) const {
        return vec ? vec->size() : 0;
    }

    template <typename T>
    size_t operator()(T) const {
        throw std::invalid_argument{"[VisitSize] Invalid type"};
    }
};
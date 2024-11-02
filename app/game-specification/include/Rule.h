#pragma once

#include "dataVariant.h"
#include <string_view>
#include <unordered_map>
#include <vector>

class Rule {
public:
    Rule(std::string_view name) : name(name) {}

    void addSpecification(std::string_view name, std::string_view value) {
        specifications[name] = value;
    }

    std::unordered_map<std::string_view, ExpressionVariant> getSpecification() {
        return specifications;
    }

private:
    std::string_view name;
    std::unordered_map<std::string_view, ExpressionVariant> specifications;
};

// NOTE: Rule specifications holds other fields (element, list, condition)
class ControlStructure : public Rule {
public:
    std::vector<std::unique_ptr<Rule>> body;
};
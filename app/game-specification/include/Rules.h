#pragma once

#include <string>
#include <cpp-tree-sitter.h>
#include "dataVariant.h"
#include <string_view>
#include <unordered_map>
#include <vector>
#include <memory>

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

class Rules {
public:
    Rules(const std::string& sourceCode);

    void parseRule(const ts::Node& node);
    void parseBody(ts::Node node);

    void addRule(const std::string_view& ruleType, const ts::Node& ruleNode);
private:
    std::string sourceCode;
    std::vector<std::unique_ptr<Rule>> rules;
};

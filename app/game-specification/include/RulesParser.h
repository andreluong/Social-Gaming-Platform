#pragma once

#include <string>
#include <cpp-tree-sitter.h>
#include "dataVariant.h"
#include <string_view>
#include <vector>
#include <memory>
#include "Rule.h"

class RulesParser {
public:
    RulesParser(const ts::Node& node, const std::string& sourceCode);

    void parseBody(const ts::Node& node, std::vector<std::unique_ptr<Rule>>& vector);
    std::unique_ptr<Rule> parseRule(const ts::Node& node);

    void print();

private:
    std::string sourceCode;
    std::vector<std::unique_ptr<Rule>> rules;
};

#pragma once

#include <string>
#include <cpp-tree-sitter.h>
#include "dataVariant.h"
#include <string_view>
#include <unordered_map>
#include <vector>
#include <memory>
#include "Rule.h"

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

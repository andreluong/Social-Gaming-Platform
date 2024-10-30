#ifndef RULES_H
#define RULES_H

#include <string>
#include <cpp-tree-sitter.h>

class Rules {
public:
    Rules(const std::string& sourceCode);

    void parseRule(const ts::Node& node) const;
    void parseBody(ts::Node node) const;
private:
    std::string sourceCode;
};

#endif
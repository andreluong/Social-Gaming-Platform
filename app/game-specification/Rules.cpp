#include "Rules.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "dataVariant.h"
#include <vector>
#include <string_view>

// TODO: Look into making a base class for all rules
// base class will have an execute function to execute its rule
// Replace int with class
std::unordered_map<std::string_view, int> allRules = {
    {"for", 1},
    {"extend", 2},
    {"discard", 3}
};

// NOTE: Temporary until variables class done
std::unordered_map<std::string_view, ExpressionVariant> variables = {
    {"winners", std::vector<std::string_view>{"Mike", "Jerry", "Tom"}},
    {"winners.size()", 2},
    {"all", std::string_view{"all the players"}}
};

// TODO: Move to variables class
// TODO: Find a way to return variable
void findVariable(std::string_view source) {
    if (auto it = variables.find(source); it != variables.end()) {
        std::visit(VisitString(), it->second);
    } else {
        std::cout << "Could not find variable: " << source << std::endl;
    }
}

Rules::Rules(const std::string& sourceCode) 
    : sourceCode(sourceCode) {}

void Rules::parseRule(const ts::Node& node) const {
    for (auto i = 0; i < node.getNumNamedChildren(); i++) {
        auto ruleNode = node.getNamedChild(i);
        auto ruleType = ruleNode.getType();

        std::cout << ruleType << std::endl << "------" << std::endl;

        if (ruleType == "for") {
            // 3 named children
            // std::cout << ruleNode.getNamedChild(0).getType() << std::endl; // element
            // std::cout << ruleNode.getNamedChild(1).getType() << std::endl; // list
            // std::cout << ruleNode.getNamedChild(2).getType() << std::endl; // body
            // TODO: Use control structure for each loop
            auto forLoopBodyNode = ruleNode.getNamedChild(2);
            parseBody(forLoopBodyNode);
        }
        else if (ruleType == "discard") {
            // 2 named children
            // Discard winners.size() from winners
            // TODO: Need variables to get
            auto count = ruleNode.getChildByFieldName("count").getSourceRange(sourceCode);
            auto source = ruleNode.getChildByFieldName("source").getSourceRange(sourceCode);
            std::cout << "count: " << count << std::endl;
            std::cout << "source: " << source << std::endl;
            findVariable(count);
            findVariable(source);
        }

        // TODO: Add more

        else if (ruleType == "body") {
            // multiple named children
            // <all> -> rule
        }
        // else if (ruleType == for){}
        else if (ruleType == "loop") {
            // 2 named children
            // condition -> expression
            // body -> body
            auto condition = ruleNode.getChildByFieldName("condition").getSourceRange(sourceCode);
            auto body = ruleNode.getChildByFieldName("body").getSourceRange(sourceCode);
            std::cout << "condition: " << condition << std::endl;
            std::cout << "body: " << body << std::endl;
            findVariable(condition);
            findVariable(body);
        }
        else if (ruleType == "parallel_for") {
            // 3 named children
            // element -> identifier
            // list -> expression
            // body -> body
        }
        else if (ruleType == "in_parallel") {
            // 1 named children
            // body -> body
        }
        else if (ruleType == "match") {
            // _ named children
            auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);
            std::cout << "target: " << target << std::endl;
            findVariable(target);
        }
        else if (ruleType == "extend") {
            // 2 named children
            auto value = ruleNode.getChildByFieldName("value").getSourceRange(sourceCode);
            auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);
            std::cout << "value: " << value << std::endl;
            std::cout << "target: " << target << std::endl;
            findVariable(value);
            findVariable(target);
        }
        else if (ruleType == "reverse") {
            // 1 named child
            auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);
            std::cout << "target: " << target << std::endl;
            findVariable(target);
        }
        else if (ruleType == "shuffle") {
            // 1 named child
            auto target = ruleNode.getChildByFieldName("target").getSourceRange(sourceCode);
            std::cout << "target: " << target << std::endl;
            findVariable(target);
        }
        else if (ruleType == "sort") {
            // _ named children
        }
        else if (ruleType == "deal") {
            // 3 named children
            auto count = ruleNode.getChildByFieldName("count").getSourceRange(sourceCode);
            auto targets = ruleNode.getChildByFieldName("targets").getSourceRange(sourceCode);
            auto source = ruleNode.getChildByFieldName("source").getSourceRange(sourceCode);
            std::cout << "count: " << count << std::endl;
            std::cout << "targets: " << targets << std::endl;
            std::cout << "source: " << source << std::endl;
            findVariable(count);
            findVariable(targets);
            findVariable(source);
        }
        else if (ruleType == "assignment") {
            // _ named children
        }
        else if (ruleType == "timer") {
            // _ named children
        }
        else if (ruleType == "input_choice") {
            // _ named children
        }
        else if (ruleType == "input_text") {
            // _ named children
        }
        else if (ruleType == "input_vote"){
            // _ named children
        }
        else if (ruleType == "input_range") {
            // _ named children
        }
        else if (ruleType == "message") {
            // 2 named children
            auto players = ruleNode.getChildByFieldName("players").getSourceRange(sourceCode);
            auto content = ruleNode.getChildByFieldName("content").getSourceRange(sourceCode);
            std::cout << "players: " << players << std::endl;
            std::cout << "content: " << content << std::endl;
            findVariable(players);
            findVariable(content);
        }
        else if (ruleType == "scores"){
            // _ named children
        }
    }
}

void Rules::parseBody(ts::Node node) const {
    for (auto i = 0; i < node.getNumNamedChildren(); i++) {
        // Children could be rule or comment (Skip comment)
        auto namedChild = node.getNamedChild(i);
        if (namedChild.getType() == "rule") {
            parseRule(namedChild);
        }
        std::cout << std::endl;
    }
}

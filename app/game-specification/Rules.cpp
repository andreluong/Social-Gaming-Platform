#include "Rules.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "dataVariant.h"
#include <vector>
#include <string_view>

std::unordered_map<std::string_view, std::size_t> operationChildren = {
    {"for", 3}, // NOTE: for testing

    {"discard", 2},
    {"extend", 2},
    {"reverse", 1},
    {"shuffle", 1},
    {"deal", 3}
    // {"sort", 2} // Varying length
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

void Rules::parseRule(const ts::Node& node) {
    for (auto i = 0; i < node.getNumNamedChildren(); i++) {
        auto ruleNode = node.getNamedChild(i);
        auto ruleType = ruleNode.getType();

        std::cout << ruleType << std::endl << "------" << std::endl;

        if (ruleType == "for") {
            addRule(ruleType, ruleNode);

            auto forLoopBodyNode = ruleNode.getNamedChild(2);
            parseBody(forLoopBodyNode);
        }
        else if (auto ruleIt = operationChildren.find(ruleType);
                ruleIt != operationChildren.end()) {
            addRule(ruleType, ruleNode);
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
        else if (ruleType == "sort") {
            // _ named children
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

void Rules::parseBody(ts::Node node) {
    for (auto i = 0; i < node.getNumNamedChildren(); i++) {
        // Children could be rule or comment (Skip comment)
        auto namedChild = node.getNamedChild(i);
        if (namedChild.getType() == "rule") {
            parseRule(namedChild);
        }
        std::cout << std::endl;
    }
}

void Rules::addRule(const std::string_view& ruleType, 
                    const ts::Node& ruleNode) {
    auto rule = std::make_unique<Rule>(ruleType);
    auto numChildren = operationChildren[ruleType]; // TODO: testing operations first

    for (std::size_t i = 0; i < numChildren; i++) {
        auto childNode = ruleNode.getNamedChild(i);
        
        // TODO: Find the actual variable and use it
        rule->addSpecification(childNode.getType(), childNode.getSourceRange(sourceCode));
        
        std::cout << "Rule: " << childNode.getType() << "; " << childNode.getSourceRange(sourceCode) << std::endl;
    }
    rules.push_back(std::move(rule));
}
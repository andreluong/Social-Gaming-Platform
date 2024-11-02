#include "RulesParser.h"
#include "Rule.h"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include "dataVariant.h"
#include <vector>
#include <string_view>

// Holds the control structure name and number of named children
std::unordered_map<std::string_view, std::size_t> controlStructureChildren = {
    {"for", 3}
};

// Holds the list operation name and number of named children
std::unordered_map<std::string_view, std::size_t> operationChildren = {
    {"discard", 2},
    {"extend", 2},
    {"reverse", 1},
    {"shuffle", 1},
    {"deal", 3}
    // {"sort", 2} // TODO: Has varying length
};

RulesParser::RulesParser(const std::string& sourceCode) 
    : sourceCode(sourceCode) {}

std::unique_ptr<ControlStructure> RulesParser::addControlStructure(const std::string_view& ruleType, const ts::Node& ruleNode) {
    auto controlStructureRule = std::make_unique<ControlStructure>(ruleType);
    auto numChildren = controlStructureChildren[ruleType];

    for (std::size_t i = 0; i < numChildren; i++) {
        auto childNode = ruleNode.getNamedChild(i);
        auto type = childNode.getType();

        if (type == "body") {
            std::cout << "Parsing body: " << childNode.getSourceRange(sourceCode) << std::endl;
            parseBody(childNode, controlStructureRule->body);
        } else {
            controlStructureRule->addSpecification(type, childNode.getSourceRange(sourceCode));
        }
    }
    return controlStructureRule;
}


std::unique_ptr<Rule> RulesParser::addListOperation(const std::string_view& ruleType, const ts::Node& ruleNode) {
    auto listOperationRule = std::make_unique<Rule>(ruleType);
    auto numChildren = operationChildren[ruleType];

    for (std::size_t i = 0; i < numChildren; i++) {
        auto childNode = ruleNode.getNamedChild(i);

        // TODO: Find the actual variable and use it
        listOperationRule->addSpecification(childNode.getType(), childNode.getSourceRange(sourceCode));
        
        std::cout << "Rule: " << childNode.getType() << "; " << childNode.getSourceRange(sourceCode) << std::endl;
    }
    return listOperationRule;
}


// Return a parsed rule
std::unique_ptr<Rule> RulesParser::parseRule(const ts::Node& node) {
    for (auto i = 0; i < node.getNumNamedChildren(); i++) {
        auto ruleNode = node.getNamedChild(i);
        auto ruleType = ruleNode.getType();

        std::cout << ruleType << std::endl << "------" << std::endl;

        if (auto controlStructureIt = controlStructureChildren.find(ruleType);
            controlStructureIt != controlStructureChildren.end()) {
            return addControlStructure(ruleType, ruleNode); // TODO: Change
        }
        else if (auto listOperationIt = operationChildren.find(ruleType);
                listOperationIt != operationChildren.end()) {
            return addListOperation(ruleType, ruleNode);
        }
    }
    // TODO: Handle error
    return nullptr;
}

void RulesParser::parseBody(const ts::Node& node, std::vector<std::unique_ptr<Rule>>& vector) {
    for (auto i = 0; i < node.getNumNamedChildren(); i++) {
        // Children could be rule or comment (Skip comment)
        auto namedChild = node.getNamedChild(i);
        if (namedChild.getType() == "rule") {
            vector.push_back(std::move(parseRule(namedChild)));
        }
        std::cout << std::endl;
    }
    std::cout << "Vector contains " << vector.size() << " elements" << std::endl;
}

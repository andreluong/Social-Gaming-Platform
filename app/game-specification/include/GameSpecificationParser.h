#pragma once

#include "SyntaxTree.h"
#include "Configuration.h"
#include "SetupRule.h"
#include "EnumDescription.h"
#include "Constants.h"
#include "ValueMap.h"
#include "Variables.h"
#include "PerPlayer.h"
#include "PerAudience.h"
// #include "PerUser.h"
#include "Rules.h" // TODO: Remove later
#include "RulesParser.h"
#include "GameManager.h"

#include <cassert>
#include <cstdio>
#include <filesystem>
#include <memory>
#include <optional>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cpp-tree-sitter.h>

enum SectionType {
    ConstantsType = 1,
    VariablesType = 2,
    PerPlayerType = 3,
    PerAudienceType = 4
};

class GameSpecificationParser {
public:
    GameSpecificationParser(const SyntaxTree& syntaxTree);

    // remove eventually

    // Objects to hold the parsed data
    //class Configuration configuration = {"", {0, 0}, true}; // Defaults
    class Constants constants;
    class Variables variables;
    class PerPlayer perPlayer;
    class PerAudience perAudience;
    // Rules rules;

    // Helper methods

    // 1 and 3 are the position of the numbers
    std::pair<int, int> parseNumberRange(ts::Node rangeNode);

    // Parse different types of expressions based on their node type
    std::string parseExpression(ts::Node expressionNode);

    /**
    * Parses a comparison operation and returns it as a string.
    *
    * Extracts the lhs expression from getChild(0)
    * Grabs the comparison op ==/!=/< from getChild(1)
    * Gets the rhs from getChild(2)
    * Combines lhs, operator, and rhs as a "lhs operator rhs" string
    */
    std::string parseComparison(ts::Node comparisonNode);

    /**
        * Parses a logical operation node, turning it into a readable string
        * 
        * - Gets the lhs from getChild(0)
        * - Extracts the logical operator (like && or ||) from getChild(1)
        * - Gets the rhs from getChild(2)
        * - Returns the expression as a string in "lhs operator rhs" format
     */
    std::string parseLogicalOperation(ts::Node logicalNode);

    // Just string to string for now; not correct yet, find comment: // Trying to figure out how to implement parseValueMap
    // Supports values of type: quoted_string, list_literal, and nested value_map
    std::unordered_map<std::string, std::string> parseValueMap(ts::Node mapNode);

    // Helper to parse a list like [ "Rock", "Paper", "Scissors" ] & ret as a formatted string
    std::string parseList(ts::Node listNode);

    // Helper to parse a nested map within a map entry ret formatted as a string
    std::string parseNestedMap(ts::Node nestedMapNode);

    // now works and parses boolean values correctly
    std::string parseBoolean(ts::Node booleanNode);

    // Parses an int node and returns it as a string
    std::string parseInteger(ts::Node integerNode);

    // Parses an Id node and returns it as a str
    std::string parseIdentifier(ts::Node identifierNode);



    // Parsing methods for each category

    Configuration parseConfiguration();



    // TODO: return valuemap?
    void parseSection(enum SectionType sectionType);

    RulesParser parseRules();

private:

    std::string sourceCode;    
    std::optional<ts::Node> root;
};
#pragma once

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

class GameSpecificationFactory {
public:
    GameSpecificationFactory(const std::string& gameFilePath);

    void parseGameSpecification() {
        parseConfiguration();
        parseConstants();
        parseVariables();
        parsePerPlayer();
        parsePerAudience();
        parseRules();
    }

    Configuration getConfiguration() {
        return configuration;
    }

    Constants getConstants() {
        return constants;
    }

    Variables getVariables() {
        return variables;
    }

    PerPlayer getPerPlayer() {
        return perPlayer;
    }

    PerAudience getPerAudience() {
        return perAudience;
    }

    // Rules getRules() {
    //     return rules;
    // }

private:

    std::string sourceCode;

    // Optional because Tree/Node have no default constructor
    std::optional<ts::Tree> tree;
    std::optional<ts::Node> root;

    // Objects to hold the parsed data
    Configuration configuration = {"", {0, 0}, true}; // Defaults
    Constants constants;
    Variables variables;
    PerPlayer perPlayer;
    PerAudience perAudience;
    // Rules rules;

    // Helper methods

    // 1 and 3 are the position of the numbers
    std::pair<int, int> parseNumberRange(ts::Node rangeNode) ;

    // Just string to string for now; not correct yet, find comment: // Trying to figure out how to implement parseValueMap
    // Supports values of type: quoted_string, list_literal, and nested value_map
    std::unordered_map<std::string, std::string> parseValueMap(ts::Node mapNode);

    // Helper to parse a list like [ "Rock", "Paper", "Scissors" ] & ret as a formatted string
    std::string parseList(ts::Node listNode);

    // Helper to parse a nested map within a map entry ret formatted as a string
    std::string parseNestedMap(ts::Node nestedMapNode);

    // Parsing methods for each category

    void parseConfiguration();

    //for parsing the constants part
    void parseConstants();

    void parseVariables();

    void parsePerPlayer();

    void parsePerAudience();

    void parseRules();
};
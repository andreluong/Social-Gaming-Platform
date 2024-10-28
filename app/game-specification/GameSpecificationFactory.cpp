#include "Configuration.h"
#include "SetupRule.h"
#include "EnumDescription.h"
#include "Constants.h"
#include "Variables.h"
#include "PerPlayer.h"
#include "PerAudience.h"
#include "Rules.h"

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

//enum class to represent different type since using string literals isn't the best
enum class NodeType {
    QuotedString,
    ListLiteral,
    ValueMap,
    Unknown
};

// A function to map string types to enums
NodeType getNodeType(const std::string& nodeType) {
    if (nodeType == "quoted_string") {
        return NodeType::QuotedString;
    } else if (nodeType == "list_literal") {
        return NodeType::ListLiteral;
    } else if (nodeType == "value_map") {
        return NodeType::ValueMap;
    } else {
        return NodeType::Unknown;
    }
}



/* TODO ideas: 

    Discuss and refine design so we can complete implementation and test

    Standardize our use of Variants

    Figure out how these classes will be used and add methods that make them useful. Use iterators to implement apparent concurrency?
    
    Is it worth it to have an interface to inherit from for some similar classes, example that stands out is
    constants, variables, perplayer, peraudience classes have same structure in grammar.js, do we even need more than one class?

    Extract method to parse different structures, like expression, expression_list, list_literal, value_map, map_entry, etc

    Lower priority:
    Read in multiple game files
    Refactor into .h and .cpp
*/

/* Notes: 

    In build folder on same level as app, run:
    cmake ../app
    make && ./game-specification/demo game-specification/games/rock-paper-scissors.game

    Make sure you have the rock-paper-scissors.game you want in app and not just in build

    To see the syntax tree with indentation (you can change the indent_size if you want in the .py main):
    Run this .cpp, it will generate sexpression.txt to the build folder, copy or move it to app/game-specification
    While in app/game-specification, run: python3 printSExpression.py sexpression.txt

    Root Node is an optional so use -> instead of . 
    Everything else can use .

    Added an extra setup_rule to RPS.game called public_voting to implement handling more than one

    Remember to add target_sources to this CMakeLists.txt

*/

extern "C" {
TSLanguage* tree_sitter_socialgaming();
}

// Temporary helper for debug
void printUnorderedMap(const std::unordered_map<std::string, std::string>& map) {
    for (const auto& pair : map) {
        std::cout << pair.first << " : " << pair.second << std::endl;
    }
}

// Parses the game file to initialize game specification objects with data, currently 6 objects (see private fields)
class GameSpecificationFactory {
public:
    GameSpecificationFactory(const std::string& gameFilePath) {

        // Load the game file
        std::ifstream gameFile(gameFilePath);
        if (!gameFile.is_open()) {
            throw std::runtime_error("Could not open game file.");
        }

        ts::Language language = tree_sitter_socialgaming();
        ts::Parser parser{language};

        std::stringstream buffer;
        buffer << gameFile.rdbuf();
        sourceCode = buffer.str();
       
        // Get the syntax tree
        tree = parser.parseString(sourceCode);

        // Get the root node of the syntax tree. 
        root = tree->getRootNode();

        if (root == std::nullopt) {
            throw std::runtime_error("Could not find root node");
        }

        // For printSExpression.py use, comment out if not using
        auto treestring = root->getSExpr();
        auto ostream = std::ofstream("sexpression.txt");
        ostream << treestring.get();
    }

    void parseGameSpecification() {
        parseConfiguration();
        parseConstants();
        // parseVariables();
        // parsePerPlayer();
        // parsePerAudience();
        // parseRules();
        
    }

    Configuration getConfiguration() {
        return configuration;
    }

    Constants getConstants() {
        return constants;
    }

    // Variables getVariables() {
    //     return variables;
    // }

    // PerPlayer getPerPlayer() {
    //     return perPlayer;
    // }

    // PerAudience getPerAudience() {
    //     return perAudience;
    // }

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
    // Variables variables;
    // PerPlayer perPlayer;
    // PerAudience perAudience;
    // Rules rules;

    // Helper methods

    // 1 and 3 are the position of the numbers
    std::pair<int, int> parseNumberRange(ts::Node rangeNode) {
        int min = std::stoi(std::string(rangeNode.getChild(1).getSourceRange(sourceCode)));
        int max = std::stoi(std::string(rangeNode.getChild(3).getSourceRange(sourceCode)));
        return {min, max};
    }

    // Just string to string for now; not correct yet, find comment: // Trying to figure out how to implement parseValueMap
    // Supports values of type: quoted_string, list_literal, and nested value_map
    std::unordered_map<std::string, std::string> parseValueMap(ts::Node mapNode) {
        std::unordered_map<std::string, std::string> valueMap;
    for (int i = 0; i < mapNode.getNumNamedChildren(); ++i) {
        ts::Node pairNode = mapNode.getNamedChild(i);
        // Ensure the pair node has a key and a value
        if (pairNode.getNumChildren() < 2) {
            std::cerr << "no children" << std::endl;
            continue;
        }
        // get the key from map entry
        std::string key = std::string(pairNode.getChildByFieldName("key").getSourceRange(sourceCode));
        ts::Node valueNode = pairNode.getChildByFieldName("value");
        std::string value;

        // for the the type of the valueNode uses the enum class that i created at the top
        NodeType type = getNodeType(std::string(valueNode.getType()));

        switch (type) {
            //i think it is unavoidable to get the feild names by their "names" for
            //TODO: try more dynamic ways to avoid this
            case NodeType::QuotedString:
                value = std::string(valueNode.getChildByFieldName("contents").getSourceRange(sourceCode));
                break;
            case NodeType::ListLiteral:
                value = parseList(valueNode);
                break;
            case NodeType::ValueMap:
                value = parseNestedMap(valueNode);
                break;
            
            default:
                // Default case, use raw text as value
                value = std::string(valueNode.getSourceRange(sourceCode));
                break;
        }

        // Debug output to confirm the parsed key-value pair
        std::cout << "Parsed keyValue pair: " << key << " : " << value << std::endl;

        // Add the key-value pair to the map
        valueMap[key] = value;
    }

    return valueMap;
}
// Helper to parse a list like [ "Rock", "Paper", "Scissors" ] & ret as a formatted string
std::string parseList(ts::Node listNode) {
    std::stringstream ss;
    ss << "[";

    for (int i = 0; i < listNode.getNumNamedChildren(); ++i) {
        ts::Node elementNode = listNode.getNamedChild(i);
        ss << elementNode.getSourceRange(sourceCode);
        if (i < listNode.getNumNamedChildren() - 1) {
            ss << ", ";
        }
    }

    ss << "]";
    return ss.str();
}

// Helper to parse a nested map within a map entry ret formatted as a string
std::string parseNestedMap(ts::Node nestedMapNode) {
    auto nestedMap = parseValueMap(nestedMapNode);
    std::stringstream ss;
    ss << "{";

    int count = 0;
    for (const auto& [key, value] : nestedMap) {
        ss << key << ": " << value;
        if (count < nestedMap.size() - 1) {
            ss << ", ";
        }
        count++;
    }

    ss << "}";
    return ss.str();
}

    // maybe refactor and add helper parsers for kind, choice, and default; and remove setters?

    // Parsing methods for each category

    void parseConfiguration() {
        
        ts::Node configurationNode = root->getChildByFieldName("configuration");

        ts::Node nameNode = configurationNode.getChildByFieldName("name");
        configuration.setName(nameNode.getSourceRange(sourceCode));
        //std::cout << configuration.getName();
     
        ts::Node playerRangeNode = configurationNode.getChildByFieldName("player_range");
    
        // new, maybe just use old for consistency unless we find a reason to refactor/remove all setter methods
        auto rangeValues = parseNumberRange(playerRangeNode);
        configuration.playerRange = rangeValues;
        // old but this works too
        //configuration.setPlayerRange(playerRangeNode.getSourceRange(sourceCode));
        //configuration.printPlayerRange();

        ts::Node hasAudienceNode = configurationNode.getChildByFieldName("has_audience");
        configuration.setHasAudience(hasAudienceNode.getSourceRange(sourceCode));
        //configuration.printHasAudience();
        
        
        // Trying to figure out how to implement parseValueMap

        // std::unordered_map<std::string, std::string> valueMap = parseValueMap(configurationNode);
        // printUnorderedMap(valueMap);
    
        // ts::Node testNode1 = root->getChildByFieldName("constants");
        // ts::Node testNode2 = testNode1.getChildByFieldName("map");
        // std::cout << testNode1.getNumChildren() << "\t" << testNode1.getNumNamedChildren() << "\t" << testNode2.getNumChildren() << "\t" << testNode2.getNumNamedChildren() << "\n";
                
        // // use testNode1 or testNode2, or something else?
        // for (int i = 0; i < testNode2.getNumChildren(); ++i) {
        //     auto pairNode = testNode2.getChild(i);
        //     std::cout << pairNode.getSourceRange(sourceCode) << "\n";
        // }

         // debug

        // std::cout << configurationNode.getNumChildren() << "\n";
        // std::cout << configurationNode.getNumNamedChildren() << "\n";


        // for (int i = 0; i < 4; i++) {
        //     std::cout << configurationNode.getNamedChild(i).getType() << "\n";
        // }

        // This is the first setup rule out of some amount, might need to be changed
        // if we can't assume there is at least one
        ts::Node setupRuleNode = configurationNode.getNamedChild(3);


        while (!setupRuleNode.isNull() && setupRuleNode.isNamed()) {

            SetupRule setupRule;

            // debug
            std::cout << setupRuleNode.getSourceRange(sourceCode) << "\n";
        
            ts::Node setupRuleNameNode = setupRuleNode.getChildByFieldName("name");
            if (!setupRuleNameNode.isNull()) {
                std::cout << "exists1\n";
                setupRule.setName(setupRuleNameNode.getSourceRange(sourceCode));
            }

             ts::Node setupRuleKindNode = setupRuleNode.getChildByFieldName("kind");
            if (!setupRuleKindNode.isNull()) {
                std::cout << "exists2\n";
                setupRule.setKind(setupRuleKindNode.getSourceRange(sourceCode));
            }

             ts::Node setupRulePromptNode = setupRuleNode.getChildByFieldName("prompt");
            if (!setupRulePromptNode.isNull()) {
                std::cout << "exists3\n";
                setupRule.setPrompt(setupRulePromptNode.getSourceRange(sourceCode));
            }

             ts::Node setupRuleRangeNode = setupRuleNode.getChildByFieldName("range");
            if (!setupRuleRangeNode.isNull()) {
                std::cout << "exists4\n";
                setupRule.setRange(setupRuleRangeNode.getSourceRange(sourceCode));
            }

            // TODO: Use EnumDescription
             ts::Node setupRuleChoicesNode = setupRuleNode.getChildByFieldName("choices");
            if (!setupRuleChoicesNode.isNull()) {
                std::cout << "exists5\n";
                setupRule.setChoices(setupRuleChoicesNode.getSourceRange(sourceCode));
            }

             ts::Node setupRuleDefaultNode = setupRuleNode.getChildByFieldName("default");
            if (!setupRuleDefaultNode.isNull()) {
                std::cout << "exists6\n";
                setupRule.setDefaultValue(setupRuleDefaultNode.getSourceRange(sourceCode));
            }

            // have not tested thoroughly yet
            configuration.addSetupRule(setupRule);

            setupRuleNode = setupRuleNode.getNextSibling();
        }

        // debug
        int count = 0;
        for (auto setupRule : configuration.getSetupRules()) {
            std::cout << count << ": " << setupRule.getName() << "\n";
            count++;
        }

    }

    // TODO: unimplemented from here on

//for parsing the constants part
void parseConstants() {
        ts::Node constantsNode = root->getChildByFieldName("constants");
        if (!constantsNode.isNull()) {
        
        // Retrieve the map node within constants
        ts::Node mapNode = constantsNode.getChildByFieldName("map");
        
        if (!mapNode.isNull()) {
            // then we can just parse the mapNode for keyVal pairs
            auto constantsMap = parseValueMap(mapNode);
            
            // then we can fill the Constants object with parsed keyVal pairs
            for (const auto& [key, value] : constantsMap) {
                constants.addConstant(key, value);
            }

            // Debug output to confirm parsed constants
            std::cout << "Parsed Constants:" << std::endl;
            for (const auto& [key, value] : constants.getConstants()) {
                std::cout << "  " << key << ": " << std::get<std::string>(value) << std::endl;
            }
            
        } else {
            std::cerr << "no constants map" << std::endl;
        }

    } else {
        std::cerr << "no constants in the file" << std::endl;
    }
}




    // void parseVariables() {
    //     auto variablesNode = root->getChildByFieldName("variables");
    //     variables = Variables(parseValueMap(variablesNode));
    // }

    // void parsePerPlayer() {
    //     auto perPlayerNode = root->getChildByFieldName("per_player");
    //     perPlayer = PerPlayer(parseValueMap(perPlayerNode));
    // }

    // void parsePerAudience() {
    //     auto perAudienceNode = root->getChildByFieldName("per_audience");
    //     perAudience = PerAudience(parseValueMap(perAudienceNode));
    // }

    // void parseRules() {
    //     auto rulesNode = root->getChildByFieldName("rules");
    //     std::string body = std::string(rulesNode.getChildByFieldName("body").getSourceRange(sourceCode));
    //     rules = Rules(body);
    // }


};

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " GAME_PATH" << '\n';
        return 1;
    }

    std::string gamePath = argv[1];

    GameSpecificationFactory factory(gamePath);

    factory.parseGameSpecification();

    // // Print syntax tree
    // auto treestring = factory.root.getSExpr();
    // printf("Syntax tree:%s\n", treestring.get());

    // // Get parsed objects
    // Configuration configuration = factory.getConfiguration();
    // Constants constants = factory.getConstants();
    // Variables variables = factory.getVariables();
    // PerPlayer perPlayer = factory.getPerPlayer();
    // PerAudience perAudience = factory.getPerAudience();
    // Rules rules = factory.getRules();


    // // Check the parsed data

    // std::cout << "Configuration Name: " << configuration.getName() << std::endl;
    // configuration.printPlayerRange();
    // configuration.printHasAudience();

    // std::cout << "Constants:" << std::endl;
    // for (const auto& [key, value] : constants.getConstants()) {
    //     std::cout << "  " << key << ": " << value << std::endl;
    // }

    // std::cout << "Variables:" << std::endl;
    // for (const auto& [key, value] : variables.getVariables()) {
    //     std::cout << "  " << key << ": " << value << std::endl;
    // }

    // std::cout << "Per Player Variables:" << std::endl;
    // for (const auto& [key, value] : perPlayer.getPerPlayerVariables()) {
    //     std::cout << "  " << key << ": " << value << std::endl;
    // }

    // std::cout << "Per Audience Variables:" << std::endl;
    // for (const auto& [key, value] : perAudience.getPerAudienceVariables()) {
    //     std::cout << "  " << key << ": " << value << std::endl;
    // }

    // std::cout << "Rules Body: " << rules.getBody() << std::endl;

    return 0;
}
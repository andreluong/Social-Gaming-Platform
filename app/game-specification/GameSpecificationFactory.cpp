#include "GameSpecificationFactory.h"


//enum class to represent different type since using string literals isn't the best
enum class NodeType {
    QuotedString,
    ListLiteral,
    ValueMap,
    Boolean,
    Integer,
    Identifier,
    Comparison,
    LogicalOperation,
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
    } else if (nodeType == "boolean") {
        return NodeType::Boolean;
    } else if (nodeType == "number") {
        return NodeType::Integer;
    } else if (nodeType == "identifier") {
        return NodeType::Identifier;
    } else if (nodeType == "comparison") {
        return NodeType::Comparison;
    } else if (nodeType == "logical_operation") {
        return NodeType::LogicalOperation;
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
GameSpecificationFactory::GameSpecificationFactory(const std::string& gameFilePath) {

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



std::string sourceCode;

// Helper methods

// 1 and 3 are the position of the numbers
std::pair<int, int> GameSpecificationFactory::parseNumberRange(ts::Node rangeNode) {
    int min = std::stoi(std::string(rangeNode.getChild(1).getSourceRange(sourceCode)));
    int max = std::stoi(std::string(rangeNode.getChild(3).getSourceRange(sourceCode)));
    return {min, max};
}

// Just string to string for now; not correct yet, find comment: // Trying to figure out how to implement parseValueMap
// Supports values of type: quoted_string, list_literal, and nested value_map
std::unordered_map<std::string, std::string> GameSpecificationFactory::parseValueMap(ts::Node mapNode) {
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
            case NodeType::Boolean:
                value = parseBoolean(valueNode);
                break;
            case NodeType::Integer:
                value = parseInteger(valueNode);
                break;
            case NodeType::Identifier:
                value = parseIdentifier(valueNode);
                break;

            default:
                // Default case, use raw text as value
                // value = std::string(valueNode.getSourceRange(sourceCode));
                /*
                * i created parseExpression and will default to it to handle any unexpected or new node types in the long run

                */
                value = parseExpression(valueNode);

                break;
        }

        // Debug output to confirm the parsed key-value pair
        std::cout << "Parsed keyValue pair: " << key << " : " << value << std::endl;

        // Add the key-value pair to the map
        valueMap[key] = value;
    }

    return valueMap;
}


// Parse different types of expressions based on their node type
std::string GameSpecificationFactory::parseExpression(ts::Node expressionNode) {
    NodeType type = getNodeType(std::string(expressionNode.getType()));

    switch (type) {
        case NodeType::Comparison:
            return parseComparison(expressionNode);
        case NodeType::LogicalOperation:
            return parseLogicalOperation(expressionNode);
        default:
            return std::string(expressionNode.getSourceRange(sourceCode));
    }
}
/**
    * Parses a comparison operation and returns it as a string.
    *
    * Extracts the lhs expression from getChild(0)
    * Grabs the comparison op ==/!=/< from getChild(1)
    * Gets the rhs from getChild(2)
    * Combines lhs, operator, and rhs as a "lhs operator rhs" string
*/

std::string GameSpecificationFactory::parseComparison(ts::Node comparisonNode) {
    std::string lhs = parseExpression(comparisonNode.getChild(0));
    std::string operatorText = std::string(comparisonNode.getChild(1).getSourceRange(sourceCode));
    std::string rhs = parseExpression(comparisonNode.getChild(2));

    return lhs + " " + operatorText + " " + rhs;
}
/**
    * Parses a logical operation node, turning it into a readable string
    * 
    * - Gets the lhs from getChild(0)
    * - Extracts the logical operator (like && or ||) from getChild(1)
    * - Gets the rhs from getChild(2)
    * - Returns the expression as a string in "lhs operator rhs" format
    */
std::string GameSpecificationFactory::parseLogicalOperation(ts::Node logicalNode) {
    std::string lhs = parseExpression(logicalNode.getChild(0));
    std::string operatorText = std::string(logicalNode.getChild(1).getSourceRange(sourceCode));
    std::string rhs = parseExpression(logicalNode.getChild(2));

    return lhs + " " + operatorText + " " + rhs;
}

// Helper to parse a list like [ "Rock", "Paper", "Scissors" ] & ret as a formatted string
std::string GameSpecificationFactory::parseList(ts::Node listNode) {
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
std::string GameSpecificationFactory::parseNestedMap(ts::Node nestedMapNode) {
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


// now works and parses boolean values correctly
std::string GameSpecificationFactory::parseBoolean(ts::Node booleanNode) {
    std::string boolValue = std::string(booleanNode.getSourceRange(sourceCode));
    if (boolValue == "true") {
        return "true";
    }
    else {
        return "false";
    }
}

// Parses an int node and returns it as a string
std::string GameSpecificationFactory::parseInteger(ts::Node integerNode) {
std::string intValueStr = std::string(integerNode.getSourceRange(sourceCode));
    // then convert str to int
    int intValue = std::stoi(intValueStr);
    // convert int back to str
    std::string result = std::to_string(intValue);
    return result;
}

// Parses an Id node and returns it as a str
std::string GameSpecificationFactory::parseIdentifier(ts::Node identifierNode) {
std::string identifierValue = std::string(identifierNode.getSourceRange(sourceCode));
    // final result in a separate variable for readability
    std::string result = identifierValue;
    return result;
}


// maybe refactor and add helper parsers for kind, choice, and default; and remove setters?

// Parsing methods for each category

void GameSpecificationFactory::parseConfiguration() {

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
        SetupRule setupRule(setupRuleNode, sourceCode);

        // have not tested thoroughly yet
        configuration.addSetupRule(setupRule);
        setupRuleNode = setupRuleNode.getNextSibling();
    }

}

// TODO: unimplemented from here on

//for parsing the constants part
void GameSpecificationFactory::parseConstants() {
        ts::Node constantsNode = root->getChildByFieldName("constants");
        if (!constantsNode.isNull()) {
        
        // Retrieve the map node within constants
        ts::Node mapNode = constantsNode.getChildByFieldName("map");
        
        if (!mapNode.isNull()) {
            // then we can just parse the mapNode for keyVal pairs
            auto constantsMap = parseValueMap(mapNode);
            
            // then we can fill the Constants object with parsed keyVal pairs
            for (const auto& [key, value] : constantsMap) {
                constants.setValue(key, value);
            }

            // Debug output to confirm parsed constants
            std::cout << "Parsed Constants:" << std::endl;
            for (const auto& [key, value] : constants.getValues()) {
                std::cout << "  " << key << ": " << std::get<std::string>(value) << std::endl;
            }
            
        } else {
            std::cerr << "no constants map" << std::endl;
        }

    } else {
        std::cerr << "no constants in the file" << std::endl;
    }
}

void GameSpecificationFactory::parseVariables() {
    auto variablesNode = root->getChildByFieldName("variables");
    // variables = Variables(parseValueMap(variablesNode));
}

void GameSpecificationFactory::parsePerPlayer() {
    auto perPlayerNode = root->getChildByFieldName("per_player");
    // perPlayer = PerUser(parseValueMap(perPlayerNode));
}

void GameSpecificationFactory::parsePerAudience() {
    auto perAudienceNode = root->getChildByFieldName("per_audience");
    // perAudience = PerAudience(parseValueMap(perAudienceNode));
}

void GameSpecificationFactory::parseRules() {
    auto rulesNode = root->getChildByFieldName("rules");
    auto bodyNode = rulesNode.getChildByFieldName("body");
    auto rulesParser = RulesParser(sourceCode);
    std::vector<std::unique_ptr<Rule>> rulesVector;
    rulesParser.parseBody(bodyNode, rulesVector);
}




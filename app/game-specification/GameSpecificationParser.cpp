#include "GameSpecificationParser.h"

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

std::unordered_map<std::string_view, NodeType> nodeTypes = {
    {"quoted_string",       NodeType::QuotedString},
    {"list_literal",        NodeType::ListLiteral},
    {"value_map",           NodeType::ValueMap},
    {"boolean",             NodeType::Boolean},
    {"number",              NodeType::Integer},
    {"identifier",          NodeType::Identifier},
    {"comparison",          NodeType::Comparison},
    {"logical_operation",   NodeType::LogicalOperation}
};

// A function to map string types to enums
NodeType getNodeType(const ts::Node& nodeType) {
    auto it = nodeTypes.find(nodeType.getType());
    if (it != nodeTypes.end()) {
        return it->second;
    }
    return NodeType::Unknown;
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



// Temporary helper for debug
void printUnorderedMap(const std::unordered_map<std::string, std::string>& map) {
    for (const auto& pair : map) {
        std::cout << pair.first << " : " << pair.second << std::endl;
    }
}

// Parses the game file to initialize game specification objects with data, currently 6 objects (see private fields)
GameSpecificationParser::GameSpecificationParser(const SyntaxTree& syntaxTree) 
    : sourceCode(syntaxTree.getSourceCode()), root(syntaxTree.getRootNode()) 
{
    constants = Constants();
    variables = Variables();
    perPlayer = PerPlayer();
    perAudience = PerAudience();
}

// Helper methods

// 1 and 3 are the position of the numbers
std::pair<int, int> GameSpecificationParser::parseNumberRange(ts::Node rangeNode) {
    int min = std::stoi(std::string(rangeNode.getChild(1).getSourceRange(sourceCode)));
    int max = std::stoi(std::string(rangeNode.getChild(3).getSourceRange(sourceCode)));
    return {min, max};
}

// Supports values of type: quoted_string, list_literal, and nested value_map
ExpressionMap GameSpecificationParser::parseValueMap(ts::Node mapNode) {
    ExpressionMap valueMap = {};
    
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
        ExpressionVariant value;
        auto type = getNodeType(valueNode);

        switch (type) {
            //i think it is unavoidable to get the feild names by their "names" for
            //TODO: try more dynamic ways to avoid this
            case NodeType::QuotedString:
                value = std::string(valueNode.getChildByFieldName("contents").getSourceRange(sourceCode));
                break;

            case NodeType::ListLiteral:
                value = std::make_shared<ExpressionVector>(parseList(valueNode));
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
                value = parseExpression(valueNode);
                break;
        }

        valueMap[key] = ExpressionWrapper{value};
    }

    return valueMap;
}


// Parse different types of expressions based on their node type
ExpressionVariant GameSpecificationParser::parseExpression(ts::Node expressionNode) {
    if (expressionNode.getNumNamedChildren() == 0) {
        throw std::runtime_error("[PARSER] Expression node does not have any named children");
    }

    auto expressionChild = expressionNode.getNamedChild(0);
    auto type = getNodeType(expressionChild);

    switch (type) {
        case NodeType::Comparison:
            return parseComparison(expressionChild);

        case NodeType::LogicalOperation:
            return parseLogicalOperation(expressionChild);

        case NodeType::Integer:
            return parseInteger(expressionChild);
        
        case NodeType::ListLiteral: {
            auto list = parseList(expressionChild);            
            return std::make_shared<ExpressionVector>(list);    
        }
        case NodeType::ValueMap: {
            auto map = parseValueMap(expressionChild);
            return std::make_shared<ExpressionMap>(map);
        }
        default:
            return std::string(expressionChild.getSourceRange(sourceCode));
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
std::string GameSpecificationParser::parseComparison(ts::Node comparisonNode) {
    auto lhs = parseExpression(comparisonNode.getChild(0));
    auto operatorText = std::string(comparisonNode.getChild(1).getSourceRange(sourceCode));
    auto rhs = parseExpression(comparisonNode.getChild(2));
    
    auto lValue = std::visit<std::string>(VisitString{}, lhs);
    auto rValue = std::visit<std::string>(VisitString{}, rhs);

    return lValue + " " + operatorText + " " + rValue;
}

/**
    * Parses a logical operation node, turning it into a readable string
    * 
    * - Gets the lhs from getChild(0)
    * - Extracts the logical operator (like && or ||) from getChild(1)
    * - Gets the rhs from getChild(2)
    * - Returns the expression as a string in "lhs operator rhs" format
    */
std::string GameSpecificationParser::parseLogicalOperation(ts::Node logicalNode) {
    auto lhs = parseExpression(logicalNode.getChild(0));
    auto operatorText = std::string(logicalNode.getChild(1).getSourceRange(sourceCode));
    auto rhs = parseExpression(logicalNode.getChild(2));

    auto lValue = std::visit<std::string>(VisitString{}, lhs);
    auto rValue = std::visit<std::string>(VisitString{}, rhs);

    return lValue + " " + operatorText + " " + rValue;
}

// Return a vector (list_literal) using the named children of a node
ExpressionVector GameSpecificationParser::parseList(ts::Node listNode) {
    ExpressionVector list;

    // Found no elements in the list, return empty list
    if (listNode.getNumNamedChildren() == 0) {
        return list;
    }

    // Add each expression to the list
    auto expressionListNode = listNode.getNamedChild(0);
    for (int i = 0; i < expressionListNode.getNumNamedChildren(); i++) {
        auto expressionListChild = expressionListNode.getNamedChild(i);
        
        // Requires 1 named child for expressions
        if (expressionListChild.getNumNamedChildren() == 0) {
            throw std::runtime_error(("[PARSER] Expression list child node does not have any named children"));
        }

        list.emplace_back(parseExpression(expressionListChild));
    }

    return list;
}

// Helper to parse a nested map within a map entry ret formatted as a string
std::string GameSpecificationParser::parseNestedMap(ts::Node nestedMapNode) {
    auto nestedMap = parseValueMap(nestedMapNode);
    std::stringstream ss;
    ss << "{";

    // TODO: Causes errors with ExpressionVariant
    // int count = 0;
    // for (const auto& [key, value] : nestedMap) {
    //     ss << key << ": " << std::visit<std::string>(VisitString{}, value);
    //     if (count < nestedMap.size() - 1) {
    //         ss << ", ";
    //     }
    //     count++;
    // }

    ss << "}";
    return ss.str();
}


// now works and parses boolean values correctly
std::string GameSpecificationParser::parseBoolean(ts::Node booleanNode) {
    std::string boolValue = std::string(booleanNode.getSourceRange(sourceCode));
    if (boolValue == "true") {
        return "true";
    }
    else {
        return "false";
    }
}

int GameSpecificationParser::parseInteger(ts::Node integerNode) {
    if (getNodeType(integerNode) == NodeType::Integer) {
        return std::stoi(std::string(integerNode.getSourceRange(sourceCode)));
    }

    throw std::invalid_argument("[PARSER] Integer node should be of type integer");
}

// Parses an Id node and returns it as a str
std::string GameSpecificationParser::parseIdentifier(ts::Node identifierNode) {
std::string identifierValue = std::string(identifierNode.getSourceRange(sourceCode));
    // final result in a separate variable for readability
    std::string result = identifierValue;
    return result;
}


// maybe refactor and add helper parsers for kind, choice, and default; and remove setters?

// Parsing methods for each category

Configuration GameSpecificationParser::parseConfiguration() {
    
    // if moving forward with refactoring, delete commented, it was moved into Configuration constructor
    ts::Node configurationNode = root->getChildByFieldName("configuration");
    Configuration configuration(configurationNode, sourceCode);
    return configuration;
}

std::unique_ptr<ValueMap> GameSpecificationParser::parseSection(enum SectionType sectionType) {
   std::unordered_map<SectionType, std::string> sectionTypeStringMap = {
        {ConstantsType, "constants"},
        {VariablesType, "variables"},
        {PerPlayerType, "per_player"},
        {PerAudienceType, "per_audience"}
    };

    std::string sectionName = sectionTypeStringMap[sectionType];
    std::unique_ptr<ValueMap> section;
    switch (sectionType) {
        case ConstantsType:
            section = std::make_unique<Constants>();
            break;
        case VariablesType:
            section = std::make_unique<Variables>();
            break;
        case PerPlayerType:
            section = std::make_unique<PerPlayer>();
            break;
        case PerAudienceType:
            section = std::make_unique<PerAudience>();
            break;
        default:
            std::cerr << "Invalid section type: " << sectionType << std::endl;
            return nullptr;
    }

    // std::cout << "Parsing section: " << sectionName << std::endl; // uncommnt to debug
    auto sectionNode = root->getChildByFieldName(sectionName);
    if (sectionNode.isNull()) {
        std::cerr << "no " << sectionName << " in the file" << std::endl;
        return nullptr;
    }
    // Retrieve the map node within sections
    ts::Node mapNode = sectionNode.getChildByFieldName("map");
    
    if (mapNode.isNull()) {
        std::cerr << "no " << sectionName << " map" << std::endl;
        return nullptr;
    }

    // then we can just parse the mapNode for keyVal pairs
    auto sectionsMap = std::make_shared<ExpressionMap>(parseValueMap(mapNode));
    // then we can fill the Sections object with parsed keyVal pairs
    for (const auto [key, wrapper] : (*sectionsMap)) {
        section->setValue(key, wrapper.value);
    }

    return section;
}

RulesParser GameSpecificationParser::parseRules() {
    auto rulesNode = root->getChildByFieldName("rules");
    auto bodyNode = rulesNode.getChildByFieldName("body");
    return RulesParser(bodyNode, sourceCode);
}




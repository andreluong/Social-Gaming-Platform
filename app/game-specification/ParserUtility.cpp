#include "ParserUtility.h"
#include "Utility.h"

namespace parserUtility {

    std::pair<int, int> parseNumberRange(const ts::Node& node, const std::string& sourceCode) {
        int min = std::stoi(std::string(node.getNamedChild(0).getSourceRange(sourceCode)));
        int max = std::stoi(std::string(node.getNamedChild(1).getSourceRange(sourceCode)));
        return {min, max};
    }
    
    bool parseBoolean(const ts::Node& booleanNode, const std::string& sourceCode) {
        auto boolValue = booleanNode.getSourceRange(sourceCode);
        return utility::evaluateBoolean(boolValue).value_or(false); // Defaults to false
    }
    
}
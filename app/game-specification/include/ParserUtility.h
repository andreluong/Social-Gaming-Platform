#pragma once

#include <utility>
#include <string>
#include <string_view>
#include <cpp-tree-sitter.h>

// NOTE: Utility class to parse simple types
namespace parserUtility {

    std::pair<int, int> parseNumberRange(const ts::Node& node, const std::string& sourceCode);

    bool parseBoolean(const ts::Node& booleanNode, const std::string& sourceCode);

}
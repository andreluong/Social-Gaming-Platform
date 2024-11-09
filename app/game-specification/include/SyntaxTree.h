#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <cpp-tree-sitter.h>

#include <optional>
#include <string>
#include <stdexcept>
#include <iostream>
// no .cpp

// Encapsulates the parsed tree, root node, and source code
class SyntaxTree {
public:
    SyntaxTree(const std::string& sourceCode, ts::Language language) 
        : sourceCode(sourceCode) {

        // Initialize parser with the provided language
        ts::Parser parser(language);

        // Parse the source code
        tree = parser.parseString(sourceCode);

        // Check if the tree was parsed correctly and initialize the root node
        if (tree.has_value()) {
            root = tree->getRootNode();
            if (!root.has_value()) {
                throw std::runtime_error("Could not find root node");
            }
        } else {
            throw std::runtime_error("Syntax tree parsing failed");
        }
    }

    const std::string& getSourceCode() const { 
        return sourceCode;
    }

    ts::Node getRootNode() const { 
        if (!root) {
            throw std::runtime_error("Root is not initialized.");
        }
        return root.value();
    }    
    
private:
    std::string sourceCode;
    std::optional<ts::Node> root;
    std::optional<ts::Tree> tree;
};

#endif
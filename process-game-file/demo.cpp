#include <cassert>
#include <cstdio>
#include <memory>
#include <string>

#include <cpp-tree-sitter.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

extern "C" {
TSLanguage* tree_sitter_json();
}

// Function to read all files in the specified directory
std::vector<std::string> readGameConfigs(const std::string& directory) {
    std::vector<std::string> gameConfigs;

    for (const auto& file : std::filesystem::directory_iterator(directory)) {
        std::ifstream file(file.path());

        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            gameConfigs.push_back(content);
            file.close();
        } else {
            std::cerr << "Failed to open file: " << file.path() << std::endl;
        }
    }
    
    return gameConfigs;
}

// incomplete, fails assert array.gettype, possibly unexpected input?: config file as string
int main() {

    std::vector<std::string> gameConfigs = readGameConfigs("games");

    // Create a language and parser.
    ts::Language language = tree_sitter_json();
    ts::Parser parser{language};

    // Parse the provided string into a syntax tree.
    std::string sourcecode = gameConfigs[0];    // modified
    ts::Tree tree = parser.parseString(sourcecode);

    // Get the root node of the syntax tree. 
    ts::Node root = tree.getRootNode();

    // Get some child nodes.
    ts::Node array = root.getNamedChild(0);
    ts::Node number = array.getNamedChild(0);

    // Check that the nodes have the expected types.
    assert(root.getType() == "document");
    assert(array.getType() == "array");
    assert(number.getType() == "number");

    // Check that the nodes have the expected child counts.
    assert(root.getNumChildren() == 1);
    assert(array.getNumChildren() == 5);
    assert(array.getNumNamedChildren() == 2);
    assert(number.getNumChildren() == 0);

    // Print the syntax tree as an S-expression.
    auto treestring = root.getSExpr();
    printf("Syntax tree: %s\n", treestring.get());

    return 0;
}

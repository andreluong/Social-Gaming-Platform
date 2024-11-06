#ifndef GAMEFILELOADER_H
#define GAMEFILELOADER_H

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "SyntaxTree.h"

// no .cpp

class GameFileLoader {
public:
    explicit GameFileLoader(const std::string& gameFilePath) : gameFilePath(gameFilePath) {}

    SyntaxTree loadSyntaxTree(ts::Language language) {
        // Load source code from the file
        std::ifstream gameFile(gameFilePath);
        if (!gameFile.is_open()) {
            throw std::runtime_error("Could not open game file.");
        }

        std::stringstream buffer;
        buffer << gameFile.rdbuf();
        std::string sourceCode = buffer.str();

        // Directly create and return SyntaxTree with sourceCode and language
        return SyntaxTree(sourceCode, language);
    }

private:
    std::string gameFilePath;
};

#endif
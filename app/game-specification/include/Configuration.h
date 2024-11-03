#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "SetupRule.h"
#include <string>
#include <vector>
#include <utility>
#include <string_view>
#include <cpp-tree-sitter.h>

// Updated to remove setters, immutable object. Constructor uses helper parsing functions and initializes fields.

class Configuration {
public:
    Configuration(ts::Node configurationNode, const std::string& sourceCode) {
        ts::Node nameNode = configurationNode.getChildByFieldName("name");
        name = nameNode.getSourceRange(sourceCode);

        ts::Node playerRangeNode = configurationNode.getChildByFieldName("player_range");
        playerRange = parseNumberRange(playerRangeNode, sourceCode);

        ts::Node hasAudienceNode = configurationNode.getChildByFieldName("has_audience");
        hasAudience = parseBool(hasAudienceNode.getSourceRange(sourceCode));

        ts::Node setupRuleNode = configurationNode.getNamedChild(3);

        while (!setupRuleNode.isNull() && setupRuleNode.isNamed()) {

            SetupRule setupRule(setupRuleNode, sourceCode);

            setupRules.emplace_back(setupRule);
            
            setupRuleNode = setupRuleNode.getNextSibling();
        }
    }

    // Public getters
    std::string getName() const { return name; }
    std::pair<int, int> getPlayerRange() const { return playerRange; }
    bool hasAudienceMembers() const { return hasAudience; }
    const std::vector<SetupRule>& getSetupRules() const { return setupRules; }

private:
    std::string name;
    std::pair<int, int> playerRange;
    bool hasAudience;
    std::vector<SetupRule> setupRules;

    // Temporary helper functions repeated just to check functionality; possibly moving them from GameSpecificationParser into static helper class or some other solution
    std::pair<int, int> parseNumberRange(const ts::Node& node, const std::string& sourceCode) {
        int min = std::stoi(std::string(node.getChild(0).getSourceRange(sourceCode)));
        int max = std::stoi(std::string(node.getChild(1).getSourceRange(sourceCode)));
        return {min, max};
    }

    bool parseBool(std::string_view str) {
        return str == "true";
    }
};

#endif

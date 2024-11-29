#include "Configuration.h"
#include "Utility.h"
#include "ParserUtility.h"

Configuration::Configuration(ts::Node configurationNode, const std::string& sourceCode) {
    auto nameNode = configurationNode.getChildByFieldName("name");
    name = nameNode.getSourceRange(sourceCode);

    auto playerRangeNode = configurationNode.getChildByFieldName("player_range");
    playerRange = parserUtility::parseNumberRange(playerRangeNode, sourceCode);

    auto hasAudienceNode = configurationNode.getChildByFieldName("has_audience");
    hasAudience = utility::evaluateBoolean(hasAudienceNode.getSourceRange(sourceCode)).value_or(false);

    // Add setup rules
    auto setupRuleNode = configurationNode.getNamedChild(3);
    while (!setupRuleNode.isNull() && setupRuleNode.isNamed()) {
        SetupRule setupRule(setupRuleNode, sourceCode);
        setupRules.emplace_back(setupRule);
        setupRuleNode = setupRuleNode.getNextSibling();
    }
}

std::string Configuration::getName() const {
    return name;
}

std::pair<int, int> Configuration::getPlayerRange() const {
    return playerRange;
}

bool Configuration::hasAudienceMembers() const {
    return hasAudience;
}

const std::vector<SetupRule>& Configuration::getSetupRules() const {
    return setupRules;
}

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "SetupRule.h"
#include <string>
#include <vector>
#include <utility>
#include <string_view>
#include <cpp-tree-sitter.h>

class Configuration {
public:
    Configuration(ts::Node configurationNode, const std::string& sourceCode);

    std::string getName() const;
    std::pair<int, int> getPlayerRange() const;
    bool hasAudienceMembers() const;
    const std::vector<SetupRule>& getSetupRules() const;

private:
    std::string name;
    std::pair<int, int> playerRange;
    bool hasAudience;
    std::vector<SetupRule> setupRules;
};

#endif

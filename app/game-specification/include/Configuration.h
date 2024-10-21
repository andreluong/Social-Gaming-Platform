#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "SetupRule.h"
#include <string>
#include <vector>
#include <utility>
#include <string_view>

class GameSpecificationFactory;

class Configuration {
public:

    Configuration(const std::string& name, std::pair<int, int> playerRange, bool hasAudience);

    // Public getters
    std::string getName() const;
    std::pair<int, int> getPlayerRange() const;
    bool hasAudienceMembers() const;
    const std::vector<SetupRule>& getSetupRules() const;


    // For debugging
    void printPlayerRange();
    void printHasAudience();


private:

    std::string name;
    std::pair<int, int> playerRange;
    bool hasAudience;
    std::vector<SetupRule> setupRules;

    // Private setters for friend access, with string parsing to convert type
    void setName(std::string_view n);
    void setPlayerRange(std::string_view rangeStr);
    void setHasAudience(std::string_view audienceStr);
    void addSetupRule(const SetupRule& rule);

    // Only friend class should access setters
    friend class GameSpecificationFactory;
};

#endif
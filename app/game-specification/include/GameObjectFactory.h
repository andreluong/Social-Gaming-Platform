#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "GameSpecificationParser.h"
#include "Configuration.h"
#include "Constants.h"
#include "Variables.h"
#include "PerAudience.h"
#include "PerPlayer.h"

#include <memory>

// no .cpp

class GameObjectFactory {
public:
    explicit GameObjectFactory(GameSpecificationParser& parser) : parser(parser) {}

    std::unique_ptr<Configuration> createConfiguration() {
        return std::make_unique<Configuration>(parser.parseConfiguration());
    }

    std::unique_ptr<Constants> createConstants() {
        return createSection<Constants>(SectionType::ConstantsType);
    }

    std::unique_ptr<Variables> createVariables() {
        return createSection<Variables>(SectionType::VariablesType);
    }

    std::unique_ptr<PerAudience> createPerAudience() {
        return createSection<PerAudience>(SectionType::PerAudienceType);
    }

    std::unique_ptr<PerPlayer> createPerPlayer() {
        return createSection<PerPlayer>(SectionType::PerPlayerType);
    }

    std::unique_ptr<RulesParser> createRules() {
        return std::make_unique<RulesParser>(parser.parseRules());
    }

private:
    GameSpecificationParser& parser;

    template <typename T>
    std::unique_ptr<T> createSection(enum SectionType sectionType) {
        auto parsedSection = parser.parseSection(sectionType);
        if (!parsedSection) {
            return nullptr;
        }
        return std::unique_ptr<T>(static_cast<T*>(parsedSection.release()));
    }

};

#endif
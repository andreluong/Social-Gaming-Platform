#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "GameSpecificationParser.h"
#include "Configuration.h"
#include "Constants.h"
#include "Variables.h"
#include "PerAudience.h"
#include "PerPlayer.h"
#include "Rules.h"

#include <memory>

// no .cpp

class GameObjectFactory {
public:
    explicit GameObjectFactory(GameSpecificationParser& parser) : parser(parser) {}

    std::unique_ptr<Configuration> createConfiguration() {
        return std::make_unique<Configuration>(parser.parseConfiguration());
    }

    std::unique_ptr<Constants> createConstants() {
        //return std::make_unique<Constants>(parser.parseSection(SectionType::ConstantsType));
    }

    std::unique_ptr<Variables> createVariables() {
        //return std::make_unique<Variables>(parser.parseSection(SectionType::VariablesType));
    }

    std::unique_ptr<PerAudience> createPerAudience() {
        //return std::make_unique<PerAudience>(parser.parseSection(SectionType::PerAudienceType));
    }

    std::unique_ptr<PerPlayer> createPerPlayer() {
        //return std::make_unique<PerPlayer>(parser.parseSection(SectionType::PerPlayerType));
    }

    std::unique_ptr<Rules> createRules() {
        //return std::make_unique<Rules>(parser.parseRules());
    }

private:
    GameSpecificationParser& parser;
};

#endif
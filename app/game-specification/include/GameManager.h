#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Configuration.h"
#include "Rule.h"
#include "GameContext.h"

/*
    configurations = get input from player on each configuration (allow json?) 
        also communicate with the server on allowed numbers players/audience to start the game

    valuemaps = stores all the variables of the game

    expression Resolver= resolves expresions 

    rule = has a update function that is non blicking

    actions rule = smallest unit of execiution

    controll rule = contains other rules, have policy for parallelism (iterrators, update, wait for IO)

    input = libs/utils/humanInput

    output = display a line

*/

/*
    // Steps:
    // load game
    // ask host to configure the game (configuration)
    // open lobby to accept players and audience depending on the config
    // once all player join, begin game.
    // complete game
    // Report the final scores?
    // end = true (reset to play again)
*/

/*

*/

// class GameManager
// {
// public:
//     GameManager(/* args */);
//     ~GameManager();

//     // propogate down to rules
//     // update()
//     // is_end()
//     // is_waiting()

//     // start_config_with_host()
//     // start_game()

// private:
//     // root: rule
//     // config: obj?
//     // variables: map<identifier, val>
//     // expression_resolver: class? // could be in rules to make this smipler
// };

// using StateMap = std::unordered_map<std::string, ValueType>;

class GameManager {
public:
    // GameManager(std::unique_ptr<Configuration> configurationParam,
    //             StateMap constantsParam,
    //             StateMap variablesParam,
    //             StateMap perPlayerParam,
    //             StateMap perAudienceParam,
    //             const std::vector<std::unique_ptr<Rule>>& rulesParam)
    //                 : configuration(std::move(configurationParam)),
    //                 constants(constantsParam),
    //                 variables(variablesParam),
    //                 perPlayer(perPlayerParam),
    //                 perAudience(perAudienceParam),
    //                 rules(rulesParam) {}

    GameManager(std::unique_ptr<Configuration> configuration,
                const std::unique_ptr<GameContext>& context, 
                const std::vector<std::unique_ptr<Rule>>& rules);

    ~GameManager() {}

    void startGame();
    void setupGame();
    void runGameLoop();
    
    // TODO: Name resolution, scope management, iterator for apparent parallelism; probably different classes
    // void resolveName(const std::string& name);

private:
    std::unique_ptr<Configuration> configuration;
    // StateMap constants;
    // StateMap variables;
    // StateMap perPlayer;
    // StateMap perAudience;
    const std::unique_ptr<GameContext>& context;
    const std::vector<std::unique_ptr<Rule>>& rules;

};


#endif
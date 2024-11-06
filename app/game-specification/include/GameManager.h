#include "GameSpecificationParser.h"

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

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

class GameManager
{
public:
    GameManager(/* args */);
    ~GameManager();

    // propogate down to rules
    // update()
    // is_end()
    // is_waiting()

    // start_config_with_host()
    // start_game()

private:
    // root: rule
    // config: obj?
    // variables: map<identifier, val>
    // expression_resolver: class? // could be in rules to make this smipler
};




#endif
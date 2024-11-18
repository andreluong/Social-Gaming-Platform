#include "GameSpecificationParser.h"
#include "SyntaxTree.h"
#include "GameFileLoader.h"
#include "GameObjectFactory.h"
#include "dataVariant.h"
#include "User.h"
#include "Server.h"

extern "C" {
TSLanguage* tree_sitter_socialgaming();
}

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " GAME_PATH" << '\n';
        return 1;
    }

    std::string gamePath = argv[1];

    GameFileLoader loader(gamePath);
    ts::Language language = tree_sitter_socialgaming();
    SyntaxTree syntaxTree = loader.loadSyntaxTree(language);

    GameSpecificationParser parser(syntaxTree);
    GameObjectFactory gameObjectFactory(parser);

    // Game objects
    std::unique_ptr<Configuration> configuration = gameObjectFactory.createConfiguration();
    std::unique_ptr<Constants> constants = gameObjectFactory.createConstants();
    std::unique_ptr<Variables> variables = gameObjectFactory.createVariables();
    std::unique_ptr<PerPlayer> perPlayer = gameObjectFactory.createPerPlayer();
    std::unique_ptr<PerAudience> perAudience = gameObjectFactory.createPerAudience();
    std::unique_ptr<RulesParser> rules = gameObjectFactory.createRules();

    // TODO: temporary map. Replace using Configuration members
    std::unordered_map<std::string, ExpressionWrapper> configurationMap;
    configurationMap["rounds"] = ExpressionWrapper{1};
    // configurationMap["public_voting"] = true;

    // Create players
    std::vector<User> players;
    User u1(1, networking::Connection((uintptr_t) 80));
    u1.addRole(std::make_shared<Player>());
    User u2(2, networking::Connection((uintptr_t) 80));
    u2.addRole(std::make_shared<Player>());
    players.push_back(u1);
    players.push_back(u2);

    // Constants
    auto configurationMapA = std::make_shared<ExpressionMap>(configurationMap);
    auto constantsA = std::make_shared<ExpressionMap>(constants->getValues());
    auto variablesA = std::make_shared<ExpressionMap>(variables->getValues());
    auto perPlayerA = std::make_shared<ExpressionMap>(perPlayer->getValues());
    auto perAudienceA = std::make_shared<ExpressionMap>(perAudience->getValues());

    auto context = std::make_unique<GameContext>(configurationMapA,
                                                constantsA, 
                                                variablesA,
                                                perPlayerA, 
                                                perAudienceA,
                                                players);

    GameManager gameManager(std::move(configuration), context, rules->getRules());

    // Temporary note: output is near the top, demo executable is in game-specification
    printf("\n// call startGame() //\n");
    gameManager.startGame();
    printf("// after call startGame() //\n\n");

    // // Check the parsed data
    // rules->print();
    // std::cout << "Printing constants values" << std::endl;
    // constants->printValues();

    // std::cout << "Printing variables values" << std::endl;
    // variables->printValues();

    // std::cout << "Printing perPlayer values" << std::endl;
    // perPlayer->printValues();

    // std::cout << "Printing perAudience values" << std::endl;
    // perAudience->printValues();
    // std::cout << "Configuration Name: " << configuration.getName() << std::endl;
    // configuration.printPlayerRange();
    // configuration.printHasAudience();

    // std::cout << "Rules Body: " << rules.getBody() << std::endl;

    printf("demo successfully ended\n");
    return 0;
}
#include "GameSpecificationParser.h"
#include "SyntaxTree.h"
#include "GameFileLoader.h"
#include "GameObjectFactory.h"


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
    // std::unique_ptr<Constants> constants = gameObjectFactory.createConstants();
    // std::unique_ptr<Variables> variables = gameObjectFactory.createVariables();
    // std::unique_ptr<PerPlayer> perPlayer = gameObjectFactory.createPerPlayer();
    // std::unique_ptr<PerAudience> perAudience = gameObjectFactory.createPerAudience();
    std::unique_ptr<RulesParser> rules = gameObjectFactory.createRules();

    // Should accept these game objects, std::move()
    // GameManager gameManager;


    // // Check the parsed data
    rules->print();

    // std::cout << "Configuration Name: " << configuration.getName() << std::endl;
    // configuration.printPlayerRange();
    // configuration.printHasAudience();

    // std::cout << "Constants:" << std::endl;
    // for (const auto& [key, value] : constants.getConstants()) {
    //     std::cout << "  " << key << ": " << value << std::endl;
    // }

    // std::cout << "Variables:" << std::endl;
    // for (const auto& [key, value] : variables.getVariables()) {
    //     std::cout << "  " << key << ": " << value << std::endl;
    // }

    // std::cout << "Per Player Variables:" << std::endl;
    // for (const auto& [key, value] : perPlayer.getPerPlayerVariables()) {
    //     std::cout << "  " << key << ": " << value << std::endl;
    // }

    // std::cout << "Per Audience Variables:" << std::endl;
    // for (const auto& [key, value] : perAudience.getPerAudienceVariables()) {
    //     std::cout << "  " << key << ": " << value << std::endl;
    // }

    // std::cout << "Rules Body: " << rules.getBody() << std::endl;

    printf("demo successfully ended\n");
    return 0;
}
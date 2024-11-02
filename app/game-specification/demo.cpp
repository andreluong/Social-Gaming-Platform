#include "GameSpecificationFactory.h"


int main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " GAME_PATH" << '\n';
        return 1;
    }

    std::string gamePath = argv[1];

    GameSpecificationFactory factory(gamePath);

    factory.parseGameSpecification();

    // // Print syntax tree
    // auto treestring = factory.root.getSExpr();
    // printf("Syntax tree:%s\n", treestring.get());

    // // Get parsed objects
    // Configuration configuration = factory.getConfiguration();
    // Constants constants = factory.getConstants();
    // Variables variables = factory.getVariables();
    // PerPlayer perPlayer = factory.getPerPlayer();
    // PerAudience perAudience = factory.getPerAudience();
    // Rules rules = factory.getRules();


    // // Check the parsed data

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

    return 0;
}
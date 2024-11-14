#include "GameManager.h"

namespace SetupHelpers {

     void printRange(const std::optional<std::pair<int, int>>& range) {

        if (range) {
            std::cout << "SetupRule range: " << range->first << "-" << range->second << "\n";
        } else {
            std::cout << "No range available.\n";
        }
    }

    void printChoices(const std::optional<std::unordered_map<std::string, std::string>>& choices) {

        if (choices) {
            std::cout << "SetupRule choices:\n";
            for (const auto& pair : *choices) {
                std::cout << pair.first << ": " << pair.second << "\n";
            }
        } else {
            std::cout << "No choices available.\n";
        }
    }

    void printDefaultValue(const std::optional<std::variant<int, bool, std::string>>& defaultValue) {

        if (defaultValue) {
            std::visit([](const auto& value) {
                std::cout << "SetupRule default value: " << value << "\n";
            }, *defaultValue);
        } else {
            std::cout << "No default value available.\n";
        }
    }
}

GameManager::GameManager(std::unique_ptr<Configuration> configuration, const std::unique_ptr<GameContext>& context, const std::vector<std::unique_ptr<Rule>>& rules) 
    : configuration(std::move(configuration)), context(std::move(context)), rules(rules) {}

void GameManager::startGame() {
    
    std::cout << "Game started: " << configuration->getName() << std::endl;
    std::cout << "Player range: " << configuration->getPlayerRange().first << "-" << configuration->getPlayerRange().second << std::endl;
    auto audienceSetting = (configuration->hasAudienceMembers()) ? "on": "off";
    std::cout << "Audience (on/off): " <<  audienceSetting << std::endl;

    setupGame();
    runGameLoop();
}

void GameManager::setupGame() {

    // Host configures the game
    std::cout << "\nConfiguring setup rules for the game... " << std::endl;

    for (SetupRule setupRule : configuration->getSetupRules()) {

        std::cout << "\nSetupRule name: " << setupRule.getName() << std::endl;
        std::cout << "SetupRule kind: " << setupRule.to_string(setupRule.getKind()) << std::endl;
        std::cout << "SetupRule prompt: " << setupRule.getPrompt() << std::endl;
    
        // Handles optional
        auto range = setupRule.getRange(); 
        auto choices = setupRule.getChoices(); 
        auto defaultValue = setupRule.getDefaultValue(); 
        SetupHelpers::printRange(range);
        SetupHelpers::printChoices(choices);
        SetupHelpers::printDefaultValue(defaultValue);

    }

}

void GameManager::runGameLoop() {

    std::cout << "\n-----------------\nRunning Game Loop\n-----------------\n" << std::endl;

    // Iterate over rules and execute them; rules should access state in constants, variables, perPlayer, perAudience

    // TODO: how do we connect rules with state so we can use iterator bookkeeping?
    for (const auto& rule : rules) {
        rule->execute(context.get());
    }
}
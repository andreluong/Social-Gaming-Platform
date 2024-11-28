#include "Rule.h"

// Old
InputChoice::InputChoice(std::string_view player, 
                        std::string_view prompt, 
                        std::string_view choices, 
                        std::string_view target) 
    : player(player), prompt(prompt), choices(choices), target(target), timeout({}) {}
    
InputChoice::InputChoice(std::string_view player, 
                        std::string_view prompt, 
                        std::string_view choices, 
                        std::string_view target, 
                        std::string_view timeout) 
    : player(player), prompt(prompt), choices(choices), target(target), timeout(timeout) {}

void InputChoice::execute(GameContext* context) {
    std::cout << "InputChoice execute" << std::endl; 
}

void InputChoice::print() {
    std::cout << "InputChoice:\nplayer: " << player << "\nprompt: " << prompt << "\nchoices: " << choices << "\ntarget: " << target << std::endl;
    if (timeout.has_value()) {
        std::cout << "timeout: " << timeout.value() << std::endl << std::endl;
    }
}

// New, not integrated yet
Input::Input(std::string_view player, 
                         std::string_view prompt, 
                         std::string_view target,
                         InputType inputType,
                         std::optional<std::string_view> timeout,
                         std::optional<std::string_view> choices,
                         std::optional<std::string_view> range)
    : player(player), prompt(prompt), target(target), inputType(inputType),
      hasTimeout(timeout), choices(choices), range(range) {}

void Input::execute(GameContext* context) {

    std::cout << "[Input] Execute" << std::endl; 
    
    // TODO: send to given player
    std::cout << "Prompt: " << prompt << std::endl; 

    // Process based on input type
    if (inputType == InputType::Choice || inputType == InputType::Vote) {
        std::cout << "Choices: ";
        for (auto& choice : *choices) {
            std::cout << choice << " ";
        }
        
    } else if (inputType == InputType::Range) {
        std::cout << "Range: " << range.value() << " to " << range.value() << std::endl;
    }

    // TODO: assign to named variable
    std::cout << "Target: " << target << std::endl; 

    if (hasTimeout.has_value()) {
        std::cout << "Timeout: " << *hasTimeout << std::endl;
    }
}

void Input::print() {
    std::cout << "Input:\nplayer: " << player << "\nprompt: " << prompt << "\nchoices: " << choices.value() << "\ntarget: " << target << std::endl;
    if (hasTimeout.has_value()) {
        std::cout << "timeout: " << hasTimeout.value() << std::endl << std::endl;
    }
}
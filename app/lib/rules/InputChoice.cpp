#include "Rule.h"

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

void InputChoice::execute() {
    std::cout << "InputChoice execute" << std::endl; 
}

void InputChoice::print() {
    std::cout << "InputChoice:\nplayer: " << player << "\nprompt: " << prompt << "\nchoices: " << choices << "\ntarget: " << target << std::endl;
    if (timeout.has_value()) {
        std::cout << "timeout: " << timeout.value() << std::endl << std::endl;
    }
}

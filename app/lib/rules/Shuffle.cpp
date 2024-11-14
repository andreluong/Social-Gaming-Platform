#include "Rule.h"

Shuffle::Shuffle(std::string_view target) 
    : target(target) {}

void Shuffle::execute(GameContext* context) {
    std::cout << "Shuffle execute" << std::endl;  
}

void Shuffle::print() {
    std::cout << "Shuffle:\ntarget: " << target << std::endl << std::endl;
}

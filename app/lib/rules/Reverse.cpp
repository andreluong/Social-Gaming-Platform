#include "Rule.h"

Reverse::Reverse(std::string_view target) 
    : target(target) {}

void Reverse::execute(GameContext* context) {
    std::cout << "Reverse execute" << std::endl;   
}

void Reverse::print() {
    std::cout << "Reverse:\ntarget: " << target << std::endl << std::endl;
}

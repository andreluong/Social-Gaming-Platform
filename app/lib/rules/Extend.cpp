#include "Rule.h"

Extend::Extend(std::string_view target, std::string_view value) 
    : target(target), value(value) {}

void Extend::execute(GameContext* context) {
    std::cout << "Extend execute" << std::endl;   
}

void Extend::print() {
    std::cout << "Extend:\ntarget: " << target << "\nvalue: " << value << std::endl << std::endl;
}

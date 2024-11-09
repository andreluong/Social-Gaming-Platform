#include "Rule.h"

Discard::Discard(std::string_view count, std::string_view source) 
    : count(count), source(source) {}

void Discard::execute() {
    std::cout << "Discard execute" << std::endl;    
}

void Discard::print() {
    std::cout << "Discard:\ncount: " << count << "\nsource: " << source << std::endl << std::endl;
}

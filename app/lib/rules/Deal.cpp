#include "Rule.h"

Deal::Deal(std::string_view count, std::string_view targets, std::string_view source) 
    : count(count), targets(targets), source(source) {}

void Deal::execute() {
    std::cout << "Deal execute" << std::endl;  
}

void Deal::print() {
    std::cout << "Deal:\ncount: " << count << "\ntargets: " << targets << "\nsource: " << source << std::endl << std::endl;
}

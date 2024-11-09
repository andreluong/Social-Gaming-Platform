#include "Rule.h"

Scores::Scores(std::string_view keys) 
    : keys(keys) {}

void Scores::execute() {
    std::cout << "Scores execute" << std::endl;
}

void Scores::print() {
    std::cout << "Scores:\nkeys: " << keys << std::endl << std::endl;
}

#include "Rule.h"

Message::Message(std::string_view players, std::string_view content)
    : players(players), content(content) {}

void Message::execute() {
    std::cout << "Message execute" << std::endl;
}

void Message::print() {
    std::cout << "Message:\nplayers: " << players << "\ncontent: " << content << std::endl << std::endl;
}

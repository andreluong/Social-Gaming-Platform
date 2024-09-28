#include <string_utils.h>

std::pair<std::string, std::string> splitCommand(const std::string& input) {
    std::istringstream iss(input);
    std::string command, argument;
    
    iss >> command;
    std::getline(iss >> std::ws, argument);
    
    return std::make_pair(command, argument);
}
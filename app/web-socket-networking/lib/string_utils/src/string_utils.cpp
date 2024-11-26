#include <string_utils.h>

std::pair<std::string, std::string> splitCommand(const std::string& input) {
    std::istringstream iss(input);
    std::string command, argument;
    
    iss >> command;
    std::getline(iss >> std::ws, argument);
    
    return std::make_pair(command, argument);
}

std::vector<std::string> splitStringBySpace(const std::string& str) {
  std::vector<std::string> result;
  std::istringstream stream(str);
  std::string word;

  // Extract each word separated by spaces
  while (stream >> word) {
      result.push_back(word);
  }

  return result;
};
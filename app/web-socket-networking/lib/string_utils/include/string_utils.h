#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <sstream>

std::pair<std::string, std::string> splitCommand(const std::string& input);

std::vector<std::string> splitStringBySpace(const std::string& str);

#endif
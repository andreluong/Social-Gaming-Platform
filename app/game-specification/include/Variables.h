#ifndef VARIABLES_H
#define VARIABLES_H

#include <unordered_map>
#include <string>

class Variables {
public:
    Variables(const std::unordered_map<std::string, std::string>& variablesMap)
        : variablesMap(variablesMap) {}

    std::unordered_map<std::string, std::string> getVariables() const {
        return variablesMap;
    }

private:
    std::unordered_map<std::string, std::string> variablesMap;
};

#endif
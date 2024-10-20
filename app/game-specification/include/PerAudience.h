#ifndef PERAUDIENCE_H
#define PERAUDIENCE_H

#include <unordered_map>
#include <string>

class PerAudience {
public:
    PerAudience(const std::unordered_map<std::string, std::string>& perAudienceMap)
        : perAudienceMap(perAudienceMap) {}

    std::unordered_map<std::string, std::string> getPerAudienceMap() const {
        return perAudienceMap;
    }

private:
    std::unordered_map<std::string, std::string> perAudienceMap;
};

#endif
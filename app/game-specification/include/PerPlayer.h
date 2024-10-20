#ifndef PERPLAYER_H
#define PERPLAYER_H

#include <unordered_map>
#include <string>

class PerPlayer {
public:
    PerPlayer(const std::unordered_map<std::string, std::string>& perPlayerMap)
        : perPlayerMap(perPlayerMap) {}

    std::unordered_map<std::string, std::string> getPerPlayerMap() const {
        return perPlayerMap;
    }

private:
    std::unordered_map<std::string, std::string> perPlayerMap;
};

#endif
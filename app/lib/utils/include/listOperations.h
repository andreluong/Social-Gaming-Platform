#pragma once

#include <vector>
#include "dataVariant.h"
#include <random>

class RandomGenerator {
public:
    virtual ~RandomGenerator() = default;
    virtual unsigned int seed() = 0;
};

class DefaultRandomGenerator : public RandomGenerator {
public:
    unsigned int seed() override {
        std::random_device rd;
        return rd();
    }
};

void extend(std::vector<dataVariant>& list, std::vector<dataVariant>& expression);
void reverse(std::vector<dataVariant>& list);
void shuffle(std::vector<dataVariant>& list, RandomGenerator& seed);
void sort(std::vector<dataVariant>& list, bool key);
void deal(std::vector<dataVariant>& playerList, std::vector<dataVariant>& expression, int num);
void discard(std::vector<dataVariant>& list, int num);

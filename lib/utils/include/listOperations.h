#pragma once

#include <vector>
#include "dataVariant.h"

void extend(std::vector<dataVariant>& list, std::vector<dataVariant>& expression);
void reverse(std::vector<dataVariant>& list);
void shuffle(std::vector<dataVariant>& list);
void sort(std::vector<dataVariant>& list, bool key);
void deal(std::vector<dataVariant>& playerList, std::vector<dataVariant>& expression, int num);
void discard(std::vector<dataVariant>& list, int num);

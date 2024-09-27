#include "listOperations.h"
#include <algorithm>
#include <random>
#include <map>

// Appends the contents of expression to list
void extend(std::vector<dataVariant>& list, std::vector<dataVariant>& expression) {
    list.insert(list.end(), expression.begin(), expression.end());
}

void reverse(std::vector<dataVariant>& list) {
    std::reverse(list.begin(), list.end());
}

void shuffle(std::vector<dataVariant>& list) {
    std::mt19937 gen(rd()); // Random generated seed
    std::shuffle(list.begin(), list.end(), gen);
}

// By default, sorting is in ascending order.
// If a key is provided, then elements of the list will be treated as maps,
// and the key will select an entry of the map to use for sorting.
void sort(std::vector<dataVariant>& list, bool key) {
    if (key != null) {
        //std::vector<std::map<std::string, dataVariant>> newList;
        return;
    }

    std::sort(list.begin(), list.end());
}

// TODO: Give X elements from expression to all players?
void deal(std::vector<dataVariant>& playerList, std::vector<dataVariant>& expression, int num) {

}

void discard(std::vector<dataVariant>& list, int num) {
    if (num > 0) {
        list.erase(list.end() - num, list.end());
    }
}

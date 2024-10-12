#pragma once

#include <vector>
#include "dataVariant.h"
#include <map>

void forEach(
    std::vector<dataVariant>& expression, 
    std::vector<dataVariant>& statementList
);

void generalLoop(bool expression, std::vector<dataVariant>& statementList);

void parallelForEach(
    std::vector<dataVariant>& expression, 
    std::vector<dataVariant>& statementList
);

void inParallel(std::vector<dataVariant>& statementList);

void match(dataVariant expression, std::map<bool, std::vector<dataVariant>> expressionToStatementList);
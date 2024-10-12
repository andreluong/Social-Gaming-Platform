#include "controlStructures.h"

void forEach(std::vector<dataVariant>& expression, std::vector<dataVariant>& statementList) {
    for (auto var : expression) {
        // Perform statement list code
        // Assume: process node from statementList
    }
}

void generalLoop(bool expression, std::vector<dataVariant>& statementList) {
    while (expression) {
        // Perform statement list code
    }
}

/*
    Executes the list of statements once for each element in the list, but it does not wait 
    for the processing of a previous element to finish before starting the next element of the list.
*/
void parallelForEach(std::vector<dataVariant>& expression, std::vector<dataVariant>& statementList) {
    
}

/*
    This starts executing each statement in a list without waiting for the previous statements
    in the list to finish. Note that this creates the potential for bugs even in a single
    threaded scenario depending on your design. It may be refined in the future.
*/
void inParallel(std::vector<dataVariant>& statementList) {

}

void match(dataVariant expression, std::map<bool, std::vector<dataVariant>> expressionToStatementList) {
    // switch (expression) {
        // map expression candidate to corresponding statement list
        /*
            expression == expressToStatement->first {
                expressToStatement->second;
            }
        */ 
    // }
}
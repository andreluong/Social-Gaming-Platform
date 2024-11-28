/*
From project description:

Inparallel
  in parallel {
    << statement list >>
  }
This starts executing each statement in a list without waiting for the previous statements in the list to finish.
Note that this creates the potential for bugs even in a single threaded scenario depending on your design. It may be refined in the future.
 */

#include "Rule.h"

InParallel::InParallel(std::vector<std::unique_ptr<ChunkableRule>> body)
    : body(std::move(body)) {}

void InParallel::execute(GameContext* context) {
    std::cout << "[InParallel] Execute" << std::endl;

    bool workRemaining = true;

    // Round-robin execution of chunks until all rules are complete
    while (workRemaining) {

        workRemaining = false;

        // Execute one chunk of each rule
        for (auto& rule : body) {

            if (!rule->isComplete()) {
                rule->executeChunk(context);
                workRemaining = true;
            }
        }
    }

    std::cout << "[InParallel] Execution complete.\n";
}

void InParallel::print() {
    // for ()
    std::cout << "Inparallel:\n "
}
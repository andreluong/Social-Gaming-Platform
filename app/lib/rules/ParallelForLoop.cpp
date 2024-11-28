/*
From project description:

Parallel for (each)
  parallel for << fresh variable >> in << expression evaluating to a list >> {
    << statement list >>
  }
Executes the list of statements once for each element in the list,
but it does not wait for the processing of a previous element to finish before starting the next element of the list.
 */

#include "Rule.h"

#include <iostream>
#include <stdexcept>

ParallelFor::ParallelFor(std::string_view element, 
                         std::string_view list, 
                         std::vector<std::unique_ptr<Rule>> body)
    : element(element), list(list), body(std::move(body)) {}

void ParallelFor::execute(GameContext* context) {

    std::cout << "[PARALLEL FOR] execute" << std::endl;

    auto contextList = context->find(list);

    if (!contextList) {
        std::cerr << "[PARALLEL FOR] No list found from context: " << list << std::endl;
        return;
    }

    auto listWrapper = contextList.value();

    // Check if it holds the expected ExpressionVector
    if (auto* vector = std::get_if<std::shared_ptr<ExpressionVector>>(&listWrapper.value)) {

        auto& expressionVector = *vector
        bool hasRemainingWork = true;

        // Initialize progress tracking of the ExpressionVector
        for (size_t i = 0; i < expressionVector->size(); ++i) {
            progress[i] = 0;
        }

        // Simulate parallel execution by interleaving time slices for each element in the list
        while (hasRemainingWork) {

            // Assume all work is done unless proven otherwise
            hasRemainingWork = false;

            for (size_t i = 0; i < expressionVector->size(); ++i) {

                auto& elementValue = (*expressionVector)[i];
                size_t currentStep = progress[i];

                // Check if there are more rules to execute for this element
                if (currentStep < body.size()) {
                    hasRemainingWork = true;

                    for (const auto& rule : body) {
                        rule->freshVariables[element] = elementValue;
                        body[currentStep]->execute(context);
                    }

                    // Move to the next step for this element
                    progress[i]++;
                }
            }

            std::cout << "[PARALLEL FOR] Iteration complete\n";
        }

    } else {
        std::cerr << "[PARALLEL FOR] List is not of expected type (ExpressionVector)" << std::endl;
    }
}

void ParallelFor::print() {
    // for ()
    std::cout << "ParallelFor:\n "
}

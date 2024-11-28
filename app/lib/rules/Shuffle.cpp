#include "Rule.h"

#include <algorithm>
#include <random>
#include <chrono>

Shuffle::Shuffle(std::string_view target) 
    : target(target) {}

void Shuffle::execute(GameContext* context) {
    std::cout << "[Shuffle] execute" << std::endl;

    auto contextTarget = context->find(target);

    if (!contextTarget || !std::holds_alternative<std::shared_ptr<ExpressionVector>>(contextTarget->value)) {
        std::cerr << "[Shuffle] Could not find target list: " << target << std::endl;
        return;
    }

    // Retrieve the target list
    auto targetList = std::get<std::shared_ptr<ExpressionVector>>(contextTarget->value);

    // Shuffle the list using std::shuffle with random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(targetList->begin(), targetList->end(), std::default_random_engine(seed));

}

void Shuffle::print() {
    std::cout << "[Shuffle] \ntarget: " << target << std::endl << std::endl;
}

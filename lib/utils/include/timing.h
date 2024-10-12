#ifndef TIMING_H
#define TIMING_H

#include <functional>
#include <vector>

// Timer modes
enum class Mode {
    NONE,
    AT_MOST,
    EXACTLY
};

// Main loop to handle timing execution time of statement list.
// Handles optional mode (default to none which does nothing to affect the actual execution time),
// and named variable (set to false at beginning of timer and true at end)
void timeStatements(int timeLimitInSeconds, const std::vector<std::function<void()>>& statementList,
                    Mode mode = Mode::NONE, bool* namedVariable = nullptr);

#endif
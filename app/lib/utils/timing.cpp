#include "include/timing.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <vector>

using namespace std::chrono;

// TODO: Maybe we can do something else during this busy wait on line 57? Do some other processing depending on how long we have to pad for

void timeStatements(int timeLimitInSeconds, const std::vector<std::function<void()>>& statementList, 
                    Mode mode, bool* namedVariable) {

    // Set named variable to false before timer expires
    if (namedVariable) {
        *namedVariable = false; 
    }

    // Record the start time
    auto start = steady_clock::now();

    // Main loop polls time elapsed to check for timer expire to break
    for (const auto& statement : statementList) {

        auto now = steady_clock::now();
        auto elapsed = duration_cast<seconds>(now - start).count();

        // Check if we've hit the time limit for "at most" mode
        if (mode == Mode::AT_MOST && elapsed >= timeLimitInSeconds) {
            //std::cout << "Time limit reached, stopping execution." << std::endl;
             // Exit the loop early
            break; 
        }

        // Execute the statement
        statement();

        // Update elapsed time after each statement
        now = steady_clock::now();
        elapsed = duration_cast<seconds>(now - start).count();

        // std::cout << "Elapsed time after statement: " << elapsed << " seconds." << std::endl;
    }

    // After completing the execution of statement list, check for "exactly" mode padding
    if (mode == Mode::EXACTLY) {
        auto now = steady_clock::now();
        auto elapsed = duration_cast<seconds>(now - start).count();

        // Pad the time to match exactly
        if (elapsed < timeLimitInSeconds) {
            auto remaining = timeLimitInSeconds - elapsed;
            // std::cout << "Padding time to match exactly " << timeLimitInSeconds << " seconds." << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(remaining));
        }
    }

    // Set named variable to true after timer expires
    if (namedVariable) {
        *namedVariable = true;
    }
}

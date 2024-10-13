#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../lib/utils/include/timing.h"

#include <chrono>
#include <thread>
#include <vector>
#include <functional>

using namespace testing;

// Example statement, one second execution time, called twice in these tests for 2 seconds actual execution time
void statement() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

class TimeStatementsTests : public testing::Test {
protected:
    bool namedVariable;

    void SetUp() override {
        namedVariable = false;
    }
};

// Test to check execution time without any mode
TEST_F(TimeStatementsTests, NoneModeTiming) {
    auto start = std::chrono::steady_clock::now();
    // timeLimitInSeconds (first argument) does not affect NONE mode
    timeStatements(0, { statement, statement});
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    
    EXPECT_GE(elapsed, 1.9) << "Expected execution time to be greater than 1.9 seconds.";
    EXPECT_LE(elapsed, 2.1) << "Expected execution time to be less than 2.1 seconds.";
}

// Test for 'at most' mode execution time
TEST_F(TimeStatementsTests, AtMostModeTiming) {
    auto start = std::chrono::steady_clock::now();
    timeStatements(2, { statement, statement }, Mode::AT_MOST);
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    
    EXPECT_LE(elapsed, 2) << "Expected execution time to be less than 2 seconds.";
}

// Test for 'exactly' mode execution time
TEST_F(TimeStatementsTests, ExactlyModeTiming) {
    auto start = std::chrono::steady_clock::now();
    timeStatements(5, { statement, statement }, Mode::EXACTLY);
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    
    EXPECT_GE(elapsed, 4.9) << "Expected execution time to be greater than 4.9 seconds.";
    EXPECT_LE(elapsed, 5.1) << "Expected execution time to be less than 5.1 seconds.";

}

// Test for named variable in 'at most' mode
TEST_F(TimeStatementsTests, NamedVariable_AtMostMode) {
    timeStatements(2, { statement, statement }, Mode::AT_MOST, &namedVariable);
    
    EXPECT_TRUE(namedVariable) << "Expected namedVariable to be true after execution.";
}

// Test for named variable in 'exactly' mode
TEST_F(TimeStatementsTests, NamedVariable_ExactlyMode) {
    timeStatements(5, { statement, statement }, Mode::EXACTLY, &namedVariable);
    
    EXPECT_TRUE(namedVariable) << "Expected timeExpired to be true after execution.";
}


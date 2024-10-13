#include "RPS.h"
#include<iostream>

RPS::RPS(int id, int maxP)
    : Game(id, "Rock Paper Scissors", maxP) {
    rules["Rock"] = "Scissors";
    rules["Paper"] = "Rock";
    rules["Scissors"] = "Paper";
}

// Initialize the game
void RPS::initGame() {
    std::cout << "Welcome to RPS Game" << std::endl;
    std::cout << "Choices available: ";
    for (const auto& choice : choices) {
        std::cout << choice << " ";
    }
    std::cout << std::endl;
}

void RPS::playRound() {
    std::cout << "Playing RPS" << std::endl;
    // for next week, I am going to add logic to collect player choice then determine the winner
}

void RPS::determineRoundWinner() {
    std::cout << "The winnder for this round is: " << std::endl;
    
}

void RPS::displayRoundResult() {
    std::cout << "Displaying round result!" << std::endl;
    // Code to display the result of the round
}

void RPS::resetGame() {
    std::cout << "Resetting and ready for a new game" << std::endl;
    //we clear the board and set current round to 0
    scoreBoard.clear();
    currentRound = 0;
}
cmake_minimum_required(VERSION 3.20)
project(SocialGamingPlatform)

# Add the game logic subdirectory
add_subdirectory(game-logic)
add_subdirectory(lib)

include(CTest)
add_subdirectory(test)

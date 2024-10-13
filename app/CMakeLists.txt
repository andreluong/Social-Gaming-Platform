cmake_minimum_required(VERSION 3.20)
project(SocialGamingPlatform)

# Add the game logic subdirectory
add_subdirectory(game-logic)
add_subdirectory(lib)
add_subdirectory(web-socket-networking)
add_subdirectory(feature-toggle)

include(CTest)
add_subdirectory(test)

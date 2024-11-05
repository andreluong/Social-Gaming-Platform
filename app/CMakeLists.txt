cmake_minimum_required(VERSION 3.20)
project(SocialGamingPlatform)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        -fsanitize=address
        -fsanitize=undefined
    )
    add_link_options(
        -fsanitize=address
        -fsanitize=undefined
    )
endif()

add_subdirectory(game-logic)
add_subdirectory(lib)
add_subdirectory(web-socket-networking)
add_subdirectory(feature-toggle)

# You can comment this out so cmake runs a bit faster
add_subdirectory(game-specification)

include(CTest)
add_subdirectory(test)

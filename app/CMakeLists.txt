cmake_minimum_required(VERSION 3.20)
project(SocialGamingPlatform)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# I added spdlog dependency
include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.11.0
)
FetchContent_MakeAvailable(spdlog)

# Enable sanitizers in Debug mode
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

# Add subdirectories for all project components
add_subdirectory(game-logic)
add_subdirectory(lib)
add_subdirectory(web-socket-networking)
add_subdirectory(feature-toggle)
add_subdirectory(chatclient-logic)

# Optional game specification directory
add_subdirectory(game-specification)

include(CTest)
add_subdirectory(test)

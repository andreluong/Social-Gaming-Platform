cmake_minimum_required(VERSION 3.28.2)
project(game-logic)

# Configure the library to be tested
add_library(game-logic-lib)
target_sources(game-logic-lib
  PRIVATE
    User.cpp
)
target_include_directories(game-logic-lib
  PUBLIC
    include/
)
set_target_properties(game-logic-lib PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 17
)

add_executable(game-logic-demo demo.cpp)

# Link the demo executable with the library
target_link_libraries(game-logic-demo PRIVATE game-logic-lib)

target_compile_features(game-logic-demo PUBLIC cxx_std_23)

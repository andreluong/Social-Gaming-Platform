cmake_minimum_required(VERSION 3.28.2)
project(feature-toggle)

# Configure the library to be tested
add_library(feature-toggle)
target_sources(feature-toggle
  PRIVATE
    featureToggle.cpp
)
target_include_directories(feature-toggle
  PUBLIC
    include/
)
set_target_properties(feature-toggle PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 17
)

# add_executable(game-logic-demo demo.cpp)

# # Link the demo executable with the library
# target_link_libraries(game-logic-demo PRIVATE game-logic-lib)

# target_compile_features(game-logic-demo PUBLIC cxx_std_23)

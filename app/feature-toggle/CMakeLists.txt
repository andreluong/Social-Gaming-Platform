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

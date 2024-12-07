cmake_minimum_required(VERSION 3.28.2)
project(chatclient-logic)

# Configure the library to be tested
add_library(chatclient-logic)
target_sources(chatclient-logic
  PUBLIC
    ChatClientLogic.cpp
)

target_include_directories(chatclient-logic
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)
set_target_properties(chatclient-logic PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 17
)

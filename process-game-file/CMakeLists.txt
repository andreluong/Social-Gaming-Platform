cmake_minimum_required(VERSION 3.28.2)

include(cmake/CPM.cmake)

# Downloads this wrapper library and tree-sitter.
# Makes them available via the `cpp-tree-sitter` CMake library target.
CPMAddPackage(
  NAME cpp-tree-sitter
  GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
  GIT_TAG v0.0.1
)

# Downloads a tree-sitter grammar from github and makes it available as a
# cmake library target.
add_grammar_from_repo(tree-sitter-json                 # Defines the library name for a grammar
  https://github.com/tree-sitter/tree-sitter-json.git  # Repository URL of a tree-sitter grammar
  0.19.0                                               # Version tag for the grammar
)



# Use the library in a demo program.
add_executable(demo)

target_sources(demo
  PRIVATE
    demo.cpp
)
target_link_libraries(demo
  tree-sitter-json
  cpp-tree-sitter
)

set(GAMES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/games")

add_custom_target(copy_games ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${GAMES_DIR}" "$<TARGET_FILE_DIR:demo>/games"
)

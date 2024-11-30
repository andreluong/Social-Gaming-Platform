# Add the gtest directory
add_subdirectory(gtest)

# Define the test executable
add_executable(runAllTests
  test_Timing.cpp
  test_ListOperations.cpp
  test_variablesAndExpressions.cpp
  test_FeatureToggle.cpp
  test_LobbyClass.cpp
  test_GameClass.cpp
  test_GameSpecificationParser.cpp
  test_GameManager.cpp  # Uncomment when ready
)

# Add include directories
target_include_directories(runAllTests
  PRIVATE
    ${CMAKE_SOURCE_DIR}/app/game-specification/include
    ${CMAKE_SOURCE_DIR}/app/utils/include
    ${CMAKE_BINARY_DIR}/_deps/cpp-tree-sitter-src/include
)

# Link libraries
target_link_libraries(runAllTests
  PRIVATE
    GTest::gmock GTest::gtest GTest::gtest_main
    game-logic-lib
    utils
    feature-toggle
    parser
    tree-sitter
)

# Add the test
add_test(NAME AllTests
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runAllTests
)
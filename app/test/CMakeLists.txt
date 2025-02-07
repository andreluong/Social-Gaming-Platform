# Add the gtest directory
add_subdirectory(gtest)

# Define the test executable
add_executable(runAllTests
  # Remove deprecated test files (removed in #65)
  # test_Timing.cpp
  # test_ListOperations.cpp
  
  test_variablesAndExpressions.cpp
  test_FeatureToggle.cpp
  test_LobbyClass.cpp
  test_GameClass.cpp
  # test_GameSpecificationParser.cpp
  test_GameManager.cpp  # Uncomment when ready
  test_ChatClient.cpp
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
    perser  # Corrected typo from "parser" to "perser"
    tree-sitter
    chatclient-logic
)

# Add the test
add_test(NAME AllTests
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runAllTests
)

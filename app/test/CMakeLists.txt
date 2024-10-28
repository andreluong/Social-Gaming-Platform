add_subdirectory(gtest)

add_executable(runAllTests

  #test_UserRoleManagement.cpp
  test_Timing.cpp
  test_ListOperations.cpp
  test_variablesAndExpressions.cpp
  test_FeatureToggle.cpp
  test_LobbyClass.cpp
  test_GameClass.cpp

  # all above passing, one is left uncommented to avoid CMake Error no sources given

  # add here
  
)

target_link_libraries(runAllTests
  PRIVATE
    GTest::gmock GTest::gtest GTest::gtest_main
    game-logic-lib
    utils
    feature-toggle
    # add here
)

add_test(NAME AllTests
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runAllTests
)
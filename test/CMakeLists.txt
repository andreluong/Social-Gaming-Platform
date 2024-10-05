add_subdirectory(gtest)

add_executable(runAllTests
  #test_UserRoleManagement.cpp
  test_Timing.cpp
  # add here
  
)

target_link_libraries(runAllTests
  PRIVATE
    GTest::gmock GTest::gtest GTest::gtest_main
    game-logic-lib
    utils
    # add here
)

add_test(NAME AllTests
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runAllTests
)


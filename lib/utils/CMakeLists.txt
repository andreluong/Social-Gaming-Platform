add_library(utils listOperations.cpp humanInput.cpp controlStructures.cpp timing.cpp)
# listOperations and humanInput currently causing build errors

target_link_libraries(utils game-logic-lib)

target_include_directories(utils
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(utils PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 20
)

install(TARGETS utils DESTINATION lib PUBLIC_HEADER DESTINATION include)

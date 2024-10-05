add_library(utils listOperations.cpp humanInput.cpp timing.cpp)
# listOperations and humanInput currently causing build errors

target_include_directories(utils
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(TARGETS utils DESTINATION lib PUBLIC_HEADER DESTINATION include)

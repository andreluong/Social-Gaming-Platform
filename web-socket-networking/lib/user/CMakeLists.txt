# Configure the library to be tested
add_library(user
    src/User.cpp
)

target_sources(user
    PRIVATE
        src/User.cpp
)

target_link_libraries(user
    PUBLIC
        lobby
)

target_include_directories(user
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

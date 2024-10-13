add_library(string_utils
    # src/lobby.cpp
)

target_sources(string_utils
    PRIVATE
        src/string_utils.cpp
)

target_include_directories(string_utils
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# install(TARGETS lobby
#   ARCHIVE DESTINATION lib 
# )
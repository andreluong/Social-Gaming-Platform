add_library(lobby
    # src/lobby.cpp
)

target_sources(lobby
    PRIVATE
        src/lobby.cpp
)

target_include_directories(lobby
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# install(TARGETS lobby
#   ARCHIVE DESTINATION lib 
# )
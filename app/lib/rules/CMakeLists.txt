add_library(rules 
  Deal.cpp
  Discard.cpp
  Extend.cpp
  ForLoop.cpp
  InputChoice.cpp
  Match.cpp
  MatchEntry.cpp
  Message.cpp
  Reverse.cpp
  Scores.cpp
  Shuffle.cpp
  WhileLoop.cpp
)

target_link_libraries(rules utils perser spdlog::spdlog)

target_include_directories(rules
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(rules PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 23
)

install(TARGETS rules DESTINATION lib PUBLIC_HEADER DESTINATION include)

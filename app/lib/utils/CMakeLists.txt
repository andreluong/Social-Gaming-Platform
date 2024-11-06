add_library(utils 
  listOperations.cpp 
  humanInput.cpp # including this causes an error with include<format> for me (Ryan), not sure why since this specifies std 23
  controlStructures.cpp 
  timing.cpp 
  sharedListFunctions.tpp 
  list.tpp 
  mapList.tpp
  inputRequestQueue.cpp
)

target_link_libraries(utils game-logic-lib)

target_include_directories(utils
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(utils PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 23
)

install(TARGETS utils DESTINATION lib PUBLIC_HEADER DESTINATION include)

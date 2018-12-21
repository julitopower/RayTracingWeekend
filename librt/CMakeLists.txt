include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/raytracing.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ray.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/material.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/render.cpp  
  )

add_library(raytracing ${SRC})
target_link_libraries(raytracing ${PNG_LIBRARIES})

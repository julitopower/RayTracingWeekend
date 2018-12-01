include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/raytracing.cpp)
add_library(raytracing ${SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../librt/include/)

add_executable(raytracer raytracer.cpp)
target_link_libraries(raytracer raytracing)

cmake_minimum_required(VERSION 2.8)
project(RayTracingWeekend CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(librt)
add_subdirectory(bin)

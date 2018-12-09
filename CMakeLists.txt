cmake_minimum_required(VERSION 2.8)
project(RayTracingWeekend CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(USE_OMP "Use omp for parallelism" ON)
if(${USE_OMP})
  add_definitions(-DUSE_OMP)
  set(CMAKE_CXX_FLAGS -fopenmp)
endif(${USE_OMP})

add_subdirectory(librt)
add_subdirectory(bin)

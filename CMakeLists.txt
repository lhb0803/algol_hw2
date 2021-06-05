cmake_minimum_required(VERSION 3.0)
project(algol_hw2)

add_compile_options(-O3)
add_compile_options(-Wall)
add_compile_options(-std=c++11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

include_directories(C:/Users/ptkjw/Documents/GitHub/algol_hw2/include)

file(GLOB SOURCES src/*)

add_subdirectory(main)
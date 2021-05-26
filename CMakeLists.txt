cmake_minimum_required(VERSION 3.0)
project(algol_hw2)

add_compile_options(-O3)
add_compile_options(-Wall)
add_compile_options(-std=c++11)

include_directories(C:/CS/Algorithm_2021-1/algol_hw2/include)

file(GLOB SOURCES src/*)

add_subdirectory(main)
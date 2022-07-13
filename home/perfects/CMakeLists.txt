cmake_minimum_required(VERSION 3.22)
project(perfects C)

enable_language(ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -fopenmp")

add_executable(perfects main.c)
target_link_libraries(perfects gmp)

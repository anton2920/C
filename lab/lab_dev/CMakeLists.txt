cmake_minimum_required(VERSION 3.8)
project(lab_dev)

enable_language(ASM)

set(CMAKE_C_STANDARD 90)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fopenmp")

# Clases
add_executable(classes classes.c)
add_executable(hamming hamming.s)

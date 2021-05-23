cmake_minimum_required(VERSION 3.8)
project(lab_dev)

set(CMAKE_C_STANDARD 90)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

set(SOURCE_FILES main.c)
set(LIBS libs/libs.c libs/libs.h)
set(HASH_MAP libs/hash_map.c libs/hash_map.h)
set(TREE_LIB libs/tree.c libs/tree.h)

add_executable(lab_dev ${SOURCE_FILES} ${LIBS}) # ${HASH_MAP} ${TREE_LIB})

target_link_libraries(lab_dev pthread rt)

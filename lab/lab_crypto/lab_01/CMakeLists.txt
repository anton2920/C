cmake_minimum_required(VERSION 3.8)
project(crypto_lab_01)

enable_language(ASM)

set(CMAKE_C_STANDARD 90)

set(LIB_CAESAR caesar.c caesar.h)
set(LIB_CMDLINE cmdline.c cmdline.h)

add_executable(caesar main.c ${LIB_CAESAR} ${LIB_CMDLINE})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
include_directories(
        src/include
        ${GLIB_INCLUDE_DIRS}
)
link_directories(${GLIB_INCLUDE_DIRS})
target_link_libraries(caesar PkgConfig::GLIB)

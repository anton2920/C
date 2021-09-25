cmake_minimum_required(VERSION 3.8)
project(lab_dev)

set(CMAKE_C_STANDARD 90)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

# Finding GLib
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0 gmodule-2.0)

set(LIB_CRYPTO_CMDLINE crypto_cmdline.c crypto_cmdline.h)
set(LIB_CRYPTO_IO crypto_io.c crypto_io.h)

add_executable(crypto crypto.c ${LIB_CRYPTO_CMDLINE} ${LIB_CRYPTO_IO})
target_include_directories(crypto PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto PkgConfig::GLIB)

# Cipher library
add_library(crypto_caesar_athens MODULE crypto_caesar_athens.c crypto_caesar_athens.h)
target_include_directories(crypto_caesar_athens PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto_caesar_athens PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto_caesar_athens PkgConfig::GLIB)

# Cracking facilities
set(LIB_CRACK_CMDLINE crack_cmdline.c crack_cmdline.h)
set(LIB_CRACK_PROCESS crack_process.c crack_process.h)

add_executable(crack crack.c ${LIB_CRACK_CMDLINE} ${LIB_CRACK_PROCESS})
target_include_directories(crack PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crack PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crack PkgConfig::GLIB)

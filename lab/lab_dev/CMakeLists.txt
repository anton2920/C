cmake_minimum_required(VERSION 3.8)
project(lab_dev)

enable_language(ASM)

set(CMAKE_C_STANDARD 90)

# Finding GLib
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0 gmodule-2.0)

set(LIB_CRYPTO_CMDLINE crypto_cmdline.c crypto_cmdline.h)
set(LIB_CRYPTO_IO crypto_io.c crypto_io.h)

add_executable(crypto crypto.c ${LIB_CRYPTO_CMDLINE} ${LIB_CRYPTO_IO})
target_include_directories(crypto PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto PkgConfig::GLIB)

# Creating separate libraries with ciphers
add_library(crypto_caesar MODULE crypto_caesar.c crypto_caesar.h)
target_include_directories(crypto_caesar PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto_caesar PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto_caesar PkgConfig::GLIB)

add_library(crypto_simple_table MODULE crypto_simple_table.c crypto_simple_table.h)
target_include_directories(crypto_simple_table PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto_simple_table PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto_simple_table PkgConfig::GLIB)

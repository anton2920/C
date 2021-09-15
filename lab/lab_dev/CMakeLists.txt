cmake_minimum_required(VERSION 3.8)
project(lab_dev)

enable_language(ASM)

set(CMAKE_C_STANDARD 90)

set(LIB_CRYPTO_CAESAR crypto_caesar.c crypto_caesar.h)
set(LIB_CRYPTO_CMDLINE crypto_cmdline.c crypto_cmdline.h)
set(LIB_CRYPTO_IO crypto_io.c crypto_io.h)

add_executable(crypto crypto.c ${LIB_CRYPTO_CMDLINE} ${LIB_CRYPTO_IO})

# Creating separate libraries with ciphers
add_library(crypto_caesar SHARED crypto_caesar.c crypto_caesar.h)
add_library(crypto_simple_table SHARED crypto_simple_table.c crypto_simple_table.h)

# Linking GLib
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0 gio-2.0 gmodule-2.0)
include_directories(
        src/include
        ${GLIB_INCLUDE_DIRS}
)
link_directories(${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto PkgConfig::GLIB)

cmake_minimum_required(VERSION 3.8)
project(crypto_lab_02)

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
add_library(crypto_vigenere MODULE crypto_vigenere.c crypto_vigenere.h)
target_include_directories(crypto_vigenere PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto_vigenere PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto_vigenere PkgConfig::GLIB)

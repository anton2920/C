cmake_minimum_required(VERSION 3.8)
project(lab_dev)

enable_language(ASM)

set(CMAKE_C_STANDARD 90)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fopenmp")

# Finding GLib
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0 gobject-2.0 gio-2.0 gmodule-2.0)

set(LIB_CRYPTO_CMDLINE crypto/crypto_cmdline.c crypto/crypto_cmdline.h)
set(LIB_CRYPTO_IO crypto/crypto_io.c crypto/crypto_io.h)

add_executable(crypto crypto/crypto.c ${LIB_CRYPTO_CMDLINE} ${LIB_CRYPTO_IO})
target_include_directories(crypto PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto PkgConfig::GLIB)

# Creating separate libraries with ciphers
add_library(crypto_caesar MODULE ciphers/crypto_caesar.c ciphers/crypto_caesar.h)
target_include_directories(crypto_caesar PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto_caesar PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto_caesar PkgConfig::GLIB)

add_library(crypto_simple_table MODULE ciphers/crypto_simple_table.c ciphers/crypto_simple_table.h)
target_include_directories(crypto_simple_table PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto_simple_table PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto_simple_table PkgConfig::GLIB)

add_library(crypto_vigenere MODULE ciphers/crypto_vigenere.c ciphers/crypto_vigenere.h)
target_include_directories(crypto_vigenere PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto_vigenere PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto_vigenere PkgConfig::GLIB)

add_library(crypto_caesar_athens MODULE ciphers/crypto_caesar_athens.c ciphers/crypto_caesar_athens.h)
target_include_directories(crypto_caesar_athens PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto_caesar_athens PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto_caesar_athens PkgConfig::GLIB)

add_library(crypto_gamma MODULE ciphers/crypto_gamma.c ciphers/crypto_gamma.h)
target_include_directories(crypto_gamma PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crypto_gamma PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crypto_gamma PkgConfig::GLIB)

set(LIB_CRACK_CMDLINE crack/crack_cmdline.c crack/crack_cmdline.h)
set(LIB_CRACK_PROCESS crack/crack_process.c crack/crack_process.h)

add_executable(crack crack/crack.c ${LIB_CRACK_CMDLINE} ${LIB_CRACK_PROCESS})
target_include_directories(crack PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(crack PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(crack PkgConfig::GLIB)

# MD5
add_executable(md5_main md5.c md5.h md5_main.c)
target_include_directories(md5_main PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(md5_main PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(md5_main PkgConfig::GLIB)

# RSA
add_executable(rsa_main rsa.c rsa.h rsa_main.c)
target_include_directories(rsa_main PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_directories(rsa_main PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(rsa_main PkgConfig::GLIB)

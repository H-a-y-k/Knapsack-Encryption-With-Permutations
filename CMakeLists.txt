cmake_minimum_required(VERSION 3.5)

project(PublicKeyEncryption LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PublicKeyEncryption main.cpp permutation.h permutation.cpp privatekeygenerator.h privatekeygenerator.cpp)

install(TARGETS PublicKeyEncryption
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

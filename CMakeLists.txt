cmake_minimum_required(VERSION 3.5)

project(PublicKeyEncryption LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PublicKeyEncryption main.cpp permutation.h privatekeygenerator.h protocol.h
    client.h
    sims.h
    basicpermutation.h
    attackmethod.h)

install(TARGETS PublicKeyEncryption
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

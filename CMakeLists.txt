cmake_minimum_required(VERSION 3.5)
project(PublicKeyEncryption LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_subdirectory(src/ EXCLUDE_FROM_ALL)
# add_subdirectory(protocol/ EXCLUDE_FROM_ALL)
# add_subdirectory(crack/ EXCLUDE_FROM_ALL)

add_executable(PublicKeyEncryption src/main.cpp util/privatekeygenerator.h protocol/protocol.h
    perm/permutation.h
    perm/basicpermutation.h
    perm/sims.h
    attack/attackmethod.h
    protocol/keypair.h
    util/permutation_vector_utils.h
    util/bitset_utils.h)
# target_include_directories(PublicKeyEncryption PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# target_include_directories(PublicKeyEncryption PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/perm)

install(TARGETS PublicKeyEncryption
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

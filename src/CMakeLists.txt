add_library(bignum)

target_sources(
    bignum
    PRIVATE
        Unsigned.cpp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/bignum/Unsigned.hpp
)

target_include_directories(
    bignum
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include/bignum
    INTERFACE
        ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(
    bignum
    PROPERTIES
        CXX_STANDARD 17
)

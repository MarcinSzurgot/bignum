project(bignum CXX)

add_library(bignum)

target_sources(
    bignum
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/bignum/IntWrapper.hpp
        # ${CMAKE_SOURCE_DIR}/include/bignum/Unsigned.hpp
        ${CMAKE_SOURCE_DIR}/include/bignum/DigitSet.hpp
)

target_include_directories(
    bignum
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include/bignum
        ${CMAKE_SOURCE_DIR}/src

    INTERFACE
        ${CMAKE_SOURCE_DIR}/include
)

# set_target_properties(
#     bignum
#     PROPERTIES
#         CXX_STANDARD 17
# )

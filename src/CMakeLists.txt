project(bignum CXX)

add_library(bignum INTERFACE)

target_sources(
    bignum
    INTERFACE
    ${CMAKE_SOURCE_DIR}/include/bignum/DigitSet.hpp
    ${CMAKE_SOURCE_DIR}/include/bignum/Operators/UnsignedComparisonOperators.hpp
    ${CMAKE_SOURCE_DIR}/include/bignum/Unsigned.hpp
)

target_include_directories(
    bignum
    INTERFACE
        ${CMAKE_SOURCE_DIR}/include
)

# set_target_properties(
#     bignum
#     PROPERTIES
#         CXX_STANDARD 17
# )

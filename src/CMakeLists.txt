project(bignum CXX)

add_library(bignum INTERFACE)

set(INTERFACE_PATH ${CMAKE_SOURCE_DIR}/include/bignum)

target_sources(
    bignum
    INTERFACE
    ${INTERFACE_PATH}/Operators/Comparison.hpp
    ${INTERFACE_PATH}/Operators/Bitshift.hpp
    ${INTERFACE_PATH}/Unsigned.hpp
    ${INTERFACE_PATH}/Utility.hpp
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

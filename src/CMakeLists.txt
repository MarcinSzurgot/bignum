project(bignum CXX)

add_library(bignum INTERFACE)

set(INTERFACE_PATH ${CMAKE_SOURCE_DIR}/include/bignum)

target_sources(
    bignum
    INTERFACE

    ${INTERFACE_PATH}/IntegerOperators/AddDiff.hpp
    ${INTERFACE_PATH}/IntegerOperators/Comparison.hpp
    ${INTERFACE_PATH}/IntegerOperators/MulDiv.hpp
    ${INTERFACE_PATH}/IntegerOperators/Stream.hpp

    ${INTERFACE_PATH}/UnsignedOperators/AddDiff.hpp
    ${INTERFACE_PATH}/UnsignedOperators/Bitshift.hpp
    ${INTERFACE_PATH}/UnsignedOperators/BitwiseLogic.hpp
    ${INTERFACE_PATH}/UnsignedOperators/Comparison.hpp
    ${INTERFACE_PATH}/UnsignedOperators/MulDiv.hpp
    ${INTERFACE_PATH}/UnsignedOperators/Stream.hpp

    ${INTERFACE_PATH}/Integer.hpp
    ${INTERFACE_PATH}/Types.hpp
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

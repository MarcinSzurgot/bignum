include(ExternalProject)
ExternalProject_Add(
    googletest
    PREFIX ${CMAKE_BINARY_DIR}/_deps
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/_install
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_BINARY_DIR}/_install/include)
link_directories(${CMAKE_BINARY_DIR}/_install/lib)

set(TEST_SOURCES
    test.cpp
    
    ConstructorsTests.cpp
    DivisionOperatorTests.cpp
    ModuloOperatorTests.cpp
    MultiplicationOperatorTests.cpp
    StringConversionTests.cpp
    Utils.hpp

    ArrayArithmeticsTests/ArrayAddTests.cpp
    ArrayArithmeticsTests/ArrayDivisionOperatorTests.cpp
    ArrayArithmeticsTests/ArrayLeftShiftOperatorTests.cpp
    ArrayArithmeticsTests/ArrayMultiplicationOperatorTests.cpp
    ArrayArithmeticsTests/ArrayRightShiftOperatorTests.cpp
    ArrayArithmeticsTests/ArraySubtractionTests.cpp
    ArrayLogicsTests/ArrayLessThanOperatorTests.cpp
)

add_executable(tests ${TEST_SOURCES})
add_dependencies(tests googletest)
target_link_libraries(tests MyLibrary gtest gtest_main pthread)

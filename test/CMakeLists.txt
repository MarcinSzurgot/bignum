include(ExternalProject)
ExternalProject_Add(
    googletest
    PREFIX ${CMAKE_BINARY_DIR}/_deps
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.10.0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/_install
)

include_directories(${CMAKE_BINARY_DIR}/_install/include)
link_directories(${CMAKE_BINARY_DIR}/_install/lib)

set(TEST_SOURCES
    test.cpp
    AdditionOperatorTests.cpp
    BinaryLeftShiftOperatorTests.cpp
    BinaryRightShiftOperatorTests.cpp
    MultiplicationOperatorTests.cpp
    SubtractionOperatorTests.cpp    
    Utils.hpp
)

add_executable(tests ${TEST_SOURCES})
add_dependencies(tests googletest)
target_link_libraries(tests MyLibrary gtest gtest_main pthread)

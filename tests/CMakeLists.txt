include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.10.0
)

FetchContent_Populate(googletest)
add_subdirectory(
    ${googletest_SOURCE_DIR}
    ${googletest_BINARY_DIR}
)

add_executable(
    tests

    main.cpp
    TestHelpers.hpp

    Integer/IntegerAddDiffTests.cpp
    Integer/IntegerComparisonTests.cpp
    Integer/IntegerStreamTests.cpp
    Integer/IntegerTests.cpp

    Unsigned/UnsignedAddDiffTests.cpp
    Unsigned/UnsignedBitshiftTests.cpp
    Unsigned/UnsignedBitwiseLogicTests.cpp
    Unsigned/UnsignedComparisonTests.cpp
    Unsigned/UnsignedMulDivTests.cpp
    Unsigned/UnsignedStreamTests.cpp
    Unsigned/UnsignedTests.cpp

    UtilityTests.cpp
)

target_link_libraries(
    tests
    gtest_main
    bignum
)

set_target_properties(
    tests
    PROPERTIES
        CXX_STANDARD 17
)

add_test(
    NAME
        tests
    COMMAND
        ${CMAKE_CURRENT_BINARY_DIR}/tests
)

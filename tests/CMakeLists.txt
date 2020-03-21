include(FetchContent)

set(OLD_CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
)

FetchContent_Populate(googletest)
add_subdirectory(
    ${googletest_SOURCE_DIR}
    ${googletest_BINARY_DIR}
)

set(CXX_ERROR_FLAGS
"-pedantic \
-Wall \
-Wcast-align \
-Wcast-qual \
-Wctor-dtor-privacy \
-Wdisabled-optimization \
-Wextra \
-Wformat=2 \
-Winit-self \
-Wlogical-op \
-Wmissing-declarations \
-Wmissing-include-dirs \
-Wnoexcept \
-Wno-unused \
-Wno-int-in-bool-context \
-Wold-style-cast \
-Woverloaded-virtual \
-Wredundant-decls \
-Wshadow \
-Wsign-conversion \
-Wsign-promo \
-Wstrict-null-sentinel \
-Wstrict-overflow=5 \
-Wundef"
)

set(CMAKE_CXX_FLAGS "${OLD_CMAKE_CXX_FLAGS} ${CXX_ERROR_FLAGS}")

add_executable(
    tests

    main.cpp
    TestHelpers.hpp

    Integer/IntegerAddDiffTests.cpp
    Integer/IntegerAllOperatorsPresentTests.cpp
    Integer/IntegerBitwiseLogicTests.cpp
    Integer/IntegerComparisonTests.cpp
    Integer/IntegerMulDivTests.cpp
    Integer/IntegerStreamTests.cpp
    Integer/IntegerTests.cpp

    Unsigned/UnsignedAddDiffTests.cpp
    Unsigned/UnsignedAllOperatorsPresentTests.cpp
    Unsigned/UnsignedBitshiftTests.cpp
    Unsigned/UnsignedBitwiseLogicTests.cpp
    Unsigned/UnsignedComparisonTests.cpp
    Unsigned/UnsignedIncrementDecrementTests.cpp
    Unsigned/UnsignedLogicOperatorTests.cpp
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

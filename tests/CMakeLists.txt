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
    UnsignedConstructorsTests.cpp
    UnsignedComparatorsTests.cpp
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

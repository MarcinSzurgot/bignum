cmake_minimum_required(VERSION 3.12)
project(bignum)

add_subdirectory(src)

option(PACKAGE_TESTS "Enable tests." ON)
if (PACKAGE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

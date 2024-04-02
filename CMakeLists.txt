cmake_minimum_required(VERSION 3.10)

add_compile_options(-fconcepts-diagnostics-depth=3)

project(MyLibrary)

add_subdirectory(lib)
add_subdirectory(test)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(MyLibrary INTERFACE)

target_include_directories(MyLibrary INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

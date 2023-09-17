set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    BigUnsigned.cpp
    Operations.cpp
)

add_library(MyLibrary ${SOURCES})

target_include_directories(MyLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Ustawiamy standard C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pliki źródłowe biblioteki
set(SOURCES
    BigUnsigned.cpp
    # ... inne pliki źródłowe ...
)

add_library(MyLibrary ${SOURCES})

# Jeśli potrzebujesz załączyć jakieś nagłówki
target_include_directories(MyLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

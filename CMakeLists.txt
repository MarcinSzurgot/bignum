cmake_minimum_required(VERSION 3.10)

project(MyLibrary)

# Dodaj podfolder z biblioteką
add_subdirectory(lib)

# Dodaj podfolder z testami
add_subdirectory(test)
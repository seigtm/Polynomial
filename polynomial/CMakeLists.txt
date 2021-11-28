# CMake basic setup.
cmake_minimum_required(VERSION 3.5)
project(Polynomial VERSION 1.5.0 LANGUAGES CXX)

# C++ 17 standart required.
set(CMAKE_CXX_STANDARD 17)

# Make the shared 'polynomial' library.
add_subdirectory(ut)
add_library(${PROJECT_NAME} SHARED polynomial.hpp)
target_include_directories(${PROJECT_NAME} INTERFACE .)

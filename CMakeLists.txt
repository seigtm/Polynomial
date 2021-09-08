cmake_minimum_required(VERSION 3.5)
project(Polynomial LANGUAGES CXX)
message("Project: " ${PROJECT_NAME})

include(CTest)

# Set the C++ parameters.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(BUILD_TESTING)
    add_subdirectory(googletest)
    add_executable(main src/main.cpp src/ut/polynomialTests.cpp)
    target_include_directories(main
        PUBLIC
            ${PROJECT_SOURCE_DIR}/polynomial
            ${PROJECT_SOURCE_DIR}/src/ut
            ${PROJECT_SOURCE_DIR}/googletest/include
    )
    target_link_libraries(main PRIVATE gtest)   # Link google test to this executable.
    add_test(NAME UnitTests COMMAND main)

    include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_html(NAME maincov EXECUTABLE main)
endif()

cmake_minimum_required(VERSION 3.5)
project(Polynomial VERSION 1.0.4 LANGUAGES CXX)
message("!- Project: " ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(polynomial)

add_executable(main src/main.cpp src/ut/polynomialTests.cpp)
target_include_directories(main
        PUBLIC
            ${PROJECT_SOURCE_DIR}/polynomial
            ${PROJECT_SOURCE_DIR}/src/ut
    )

target_link_libraries(main PRIVATE CONAN_PKG::gtest CONAN_PKG::boost)
message("!- Conan libs: ${CONAN_LIBS}")

if (CODE_COVERAGE)
    message("!- Using code coverage")
    # Use CodeCoverage for coverage reports.
    include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_html(NAME maincov EXECUTABLE main)
endif()

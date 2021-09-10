cmake_minimum_required(VERSION 3.5)
project(Polynomial VERSION 1.0.1 LANGUAGES CXX)
message("!- Project: " ${PROJECT_NAME})

include(CTest)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    message(FATAL_ERROR "conanbuildinfo.cmake required is missing")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGET)

set(CMAKE_CXX_STANDARD 17)

if(BUILD_TESTING)
    add_executable(main src/main.cpp src/ut/polynomialTests.cpp)
    target_include_directories(main
        PUBLIC
            ${PROJECT_SOURCE_DIR}/polynomial
            ${PROJECT_SOURCE_DIR}/src/ut
    )

    include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)
    append_coverage_compiler_flags()
    
    # Turn off optimization for non-skewed coverage reports.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    setup_target_for_coverage_gcovr_html(NAME maincov EXECUTABLE main)

    target_link_libraries(main PRIVATE ${CONAN_LIBS})
    message("!- Conan libs: ${CONAN_LIBS}")
endif()
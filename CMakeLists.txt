# CMake basic setup.
cmake_minimum_required(VERSION 3.5)
project(PolynomialExample VERSION 2.0.0 LANGUAGES CXX)

# Build shared gtest library.
if (NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()

# Linker flag to search for libPolynomial in executable path.
SET(CMAKE_EXE_LINKER_FLAGS 
    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,."
)

# C++ 17 standart required.
set(CMAKE_CXX_STANDARD 17)

# Add subdirectories to the build.
add_subdirectory(polynomial)
add_subdirectory(googletest)

# Add 'main' source to the main executable.
add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
)

# Link Polynomial lib to the executable.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Polynomial
)

# Before create "main" build target "Run_PolynomialTests".
add_dependencies(${CMAKE_PROJECT_NAME} Run_PolynomialTests)

# Deploy project.
if (DEPLOY)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_SOURCE_DIR}/scripts/deploy.sh"
            "${CMAKE_SOURCE_DIR}/build/PolynomialExample"          # Executable path.
            "${CMAKE_SOURCE_DIR}/build/polynomial/libPolynomial.so"   # Library path.
            "${CMAKE_SOURCE_DIR}/deploy"                        # Destination path.
        COMMENT "Running deploy"
    )
endif()
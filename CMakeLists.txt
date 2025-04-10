cmake_minimum_required(VERSION 3.16)
project(dquant LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Recommended: out-of-source builds only
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds are not allowed. Please create a separate build directory.")
endif()

# Add Catch2 (manually cloned into external/)
add_subdirectory(external/Catch2)

# Add source subdirectories (each is a module or component)
add_subdirectory(src/core)
add_subdirectory(src/rhythmizer)
add_subdirectory(src/dynamizer)
add_subdirectory(src/intonizer)
add_subdirectory(src/main)

# Add test suite subdirectories
add_subdirectory(tests/unit)
add_subdirectory(tests/regression)

# Add analysis tools
add_subdirectory(tools/analyze_rhythm)
add_subdirectory(tools/analyze_dynamics)


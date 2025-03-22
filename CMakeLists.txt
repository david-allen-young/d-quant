cmake_minimum_required(VERSION 3.16)
project(DQuant LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Recommended: out-of-source builds only
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds are not allowed. Please create a separate build directory.")
endif()

# Add source subdirectories (each is a module or component)
add_subdirectory(src/core)
add_subdirectory(src/rhythmizer)
add_subdirectory(src/dynamizer)
add_subdirectory(src/intonizer)
add_subdirectory(src/main)

# Optional: Add test suite later
# add_subdirectory(tests/unit)

add_library(intonizer
    intonizer.cpp
    IntonationGenerator.cpp
)

target_include_directories(intonizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(intonizer PUBLIC core)

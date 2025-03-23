add_library(rhythmizer
    rhythmizer.cpp
)

target_include_directories(rhythmizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rhythmizer PUBLIC core)

add_library(rhythmizer
    rhythmizer.cpp
    rhythmizer.h
)

target_include_directories(rhythmizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rhythmizer PUBLIC core)

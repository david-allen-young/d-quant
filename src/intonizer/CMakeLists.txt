add_library(intonizer
    intonizer.cpp
    intonizer.h
)

target_include_directories(intonizer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(intonizer PUBLIC core)

add_library(core
    midi_parser.cpp
    midi_parser.h
    # Add other core source/header files here
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

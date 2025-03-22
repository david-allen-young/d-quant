add_library(core
    midi_parser.cpp
    # Add other core source/header files here
	MidiFileReader_v2.cpp
    MidiTrackParser.cpp
    NoteBuilder.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

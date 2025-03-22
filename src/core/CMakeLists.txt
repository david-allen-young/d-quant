add_library(core
    midi_parser.cpp
    midi_parser.h
    # Add other core source/header files here
	MidiFileReader_v2.cpp
    MidiFileReader_v2.h
    MidiTrackParser.cpp
    MidiTrackParser.h
    NoteBuilder.cpp
    NoteBuilder.h
    Note.h
    Point.h
    AnalysisHelpers.h
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

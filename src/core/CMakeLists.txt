add_library(core
    midi_parser.cpp
	AnalysisHelpers.cpp
	MidiFileReader_v2.cpp
	MidiFileWriter.cpp
    MidiTrackParser.cpp
    NoteBuilder.cpp
	PipelineArgsRegistry.cpp
	PipelineConfigLoader.cpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(core PRIVATE ${PROJECT_SOURCE_DIR}/external/json)

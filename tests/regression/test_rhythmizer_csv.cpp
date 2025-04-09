#include "MidiFileReader_v2.h"
#include "MidiTrackParser.h"
#include "RhythmAnalysis.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "PipelineArgsRegistry.h"
namespace fs = std::filesystem;
using namespace rhythmizer;

TEST_CASE("Rhythmizer processes training MIDI file and writes deviation CSV", "[rhythmizer]")
{
    const auto& args = getPipelineArgs();
    const std::string inputFile = args.rhythmizer_midi_path;
    const std::string outFile = args.rhythm_deviation_csv;
    
    // Ensure parent directory of output CSV exists
    fs::create_directories(fs::path(outFile).parent_path());

    MidiFileReader_v2 midi;
    midi.readMidi(inputFile);
    REQUIRE(midi.getTracks().size() > 1);

    std::vector<NoteData> notes;
    MidiTrackParser parser;
    parser.extractNoteEvents(midi.getTracks()[1], notes); // test track 1

    CHECK(notes.size() > 0);

    writeRhythmDeviationCSV(notes, outFile);
    CHECK(fs::exists(outFile));

    std::ifstream f(outFile);
    std::string header;
    std::getline(f, header);
    CHECK(header == "NominalPosition,DeltaPosition,DurationRatio,VelocityDelta");

    int lineCount = 0;
    std::string line;
    while (std::getline(f, line))
    {
        CHECK(!line.empty());
        ++lineCount;
    }

    CHECK(lineCount == notes.size());
    std::cout << "Wrote: " << fs::absolute(outFile) << std::endl;
}

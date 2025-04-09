#include "MidiFileReader_v2.h"
#include "MidiTrackParser.h"
#include "RhythmAnalysis.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
using namespace rhythmizer;

TEST_CASE("Rhythmizer processes training MIDI file and writes deviation CSV", "[rhythmizer]")
{
    const std::string inputFile = "C:/GitHub/d-quant/assets/midi/sample_training_file_rhythmizer.mid";
    const std::string outDir = "test_output/rhythmizer";
    const std::string outFile = outDir + "/deviation_from_midi.csv";
    fs::create_directories(outDir);

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

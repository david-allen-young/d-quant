#include "dynamizer.h"
#include "AnalysisHelpers.h"
#include "MidiFileReader_v2.h"
#include "MidiTrackParser.h"

#include <iostream>
#include <vector>

void run_envelope_csv_example()
{
    std::vector<Point> envelope = {
        {0.0, 0.0}, {0.5, 0.10}, {1.0, 0.15}, {1.5, 0.20}, {2.0, 0.25}, {2.5, 0.35}, {3.0, 0.47}, {3.5, 0.63}, {4.0, 0.89}};

    std::string file = "assets/output_csv/envelope.csv";
    writeCSV(envelope, file);
}

void run_midi_to_envelope_csv(const std::string& midiFilePath, const std::string& outputDir)
{
    MidiFileReader_v2 midiFile;
    midiFile.readMidi(midiFilePath);
    std::cout << "Reading " << midiFilePath << std::endl;

    auto tracks = midiFile.getTracks();
    for (int trackIdx = 1; trackIdx < tracks.size(); ++trackIdx)
    {
        std::cout << "Track " << trackIdx << " data:\n";

        MidiTrackParser parser;
        std::vector<Point> breathEvents;
        parser.parseTrackEvents(tracks[trackIdx], breathEvents);

        std::string file = outputDir + "/envelope" + std::to_string(trackIdx) + ".csv";
        writeCSV(breathEvents, file);
        std::cout << "Wrote " << file << std::endl;
    }
}

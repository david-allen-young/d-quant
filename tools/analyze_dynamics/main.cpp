#include "AnalysisHelpers.h"
#include "MidiFileReader_v2.h"
#include "MidiTrackParser.h"
#include "PipelineArgsRegistry.h"

#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    initPipelineArgs(argc, argv);
    const auto& args = getPipelineArgs();

    std::cout << "Reading dynamics training MIDI: " << args.dynamizer_midi_path << "\n";
    if (!std::filesystem::exists(args.dynamizer_midi_path))
    {
        std::cerr << "[ERROR] File not found: " << args.dynamizer_midi_path << "\n";
        return 1;
    }

    MidiFileReader_v2 midiFile;
    midiFile.readMidi(args.dynamizer_midi_path);

    /*const*/ auto& tracks = midiFile.getTracks();
    if (tracks.size() < 2)
    {
        std::cerr << "[ERROR] No usable tracks found in MIDI file.\n";
        return 1;
    }

    std::filesystem::create_directories(args.envelope_csv_dir);
    std::cout << "Saving envelopes to: " << args.envelope_csv_dir << "\n";

    for (size_t i = 1; i < tracks.size(); ++i)
    {
        std::vector<Point> breathEnvelope;
        MidiTrackParser parser;
        parser.parseTrackEvents(tracks[i], breathEnvelope);

        if (breathEnvelope.empty())
        {
            std::cout << "Track " << i << ": No breath data found.\n";
            continue;
        }

        std::string outPath = args.envelope_csv_dir + "/envelope" + std::to_string(i) + ".csv";
        writeCSV(breathEnvelope, outPath);
        std::cout << "Track " << i << ": Wrote " << outPath << " (" << breathEnvelope.size() << " points)\n";
    }

    return 0;
}

//#include "AnalysisHelpers.h"
#include "MidiFileReader_v2.h"
#include "MidiTrackParser.h"
#include "Note.h"
#include "Point.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: analyze_rhythm <input_midi_file>\n";
        return 1;
    }

    std::string midiPath = argv[1];
    std::cout << "Reading MIDI file: " << midiPath << "\n";

    MidiFileReader_v2 reader;
    reader.readMidi(midiPath);
    const auto& tracks = reader.getTracks();

    if (tracks.size() < 2)
    {
        std::cerr << "Error: Not enough tracks in MIDI file.\n";
        return 1;
    }

    // We'll process the first non-empty track
    std::vector<char> trackData = tracks[1]; // or choose more intelligently

    // Parse note-on/note-off pairs
    MidiTrackParser parser;
    std::vector<NoteData> notes;
    parser.extractNoteEvents(trackData, notes); // you'll implement this helper

    // Sort by start time
    std::sort(notes.begin(), notes.end(),
              [](const NoteData& a, const NoteData& b)
              { return a.positionInBeats < b.positionInBeats; });

    // Generate expected positions: 1.0, 2.0, ...
    std::vector<Point> rhythmDeviations;
    for (size_t i = 0; i < notes.size(); ++i)
    {
        double nominalStart = 1.0 + static_cast<double>(i); // grid start
        double nominalDuration = 1.0;

        double deltaPos = notes[i].positionInBeats - nominalStart;
        double durationRatio = notes[i].durationInBeats / nominalDuration;

        rhythmDeviations.emplace_back(deltaPos, durationRatio);
        std::cout << "Note " << i << ": deltaPos = " << deltaPos
                  << ", ratio = " << durationRatio << "\n";
    }

    // Write to CSV
    std::filesystem::create_directories("assets/output_csv/");
    std::string outPath = "assets/output_csv/rhythm_deviation.csv";
    std::ofstream out(outPath);
    out << "DeltaPosition,DurationRatio\n";
    for (const auto& [delta, ratio] : rhythmDeviations)
    {
        out << std::fixed << std::setprecision(5) << delta << "," << ratio << "\n";
    }
    out.close();

    std::cout << "Wrote rhythm deviation CSV to: " << outPath << "\n";
    return 0;
}

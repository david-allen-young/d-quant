#include "PhraseRenderer.h"
#include <cmath>
#include <iostream>
#include <map>

namespace
{

// MIDI note conversion (e.g., "C4" → 60)
int pitchToMidi(const std::string& name)
{
    static const std::map<std::string, int> baseNotes = {
        {"C", 0}, {"C#", 1}, {"D", 2}, {"D#", 3}, {"E", 4}, {"F", 5}, {"F#", 6}, {"G", 7}, {"G#", 8}, {"A", 9}, {"A#", 10}, {"B", 11}};

    if (name.size() < 2)
        return 60; // default to C4
    std::string letter = name.substr(0, name.length() - 1);
    int octave = name.back() - '0';
    auto it = baseNotes.find(letter);
    if (it == baseNotes.end())
        return 60;

    return 12 * (octave + 1) + it->second;
}

} // anonymous namespace

void generate_phrase_midi(const PhraseArgs& phrase, const SongContext& context, const RenderOptions& options)
{
    std::cout << "[DEBUG] Output ID: " << options.output_id << "\n";
    std::cout << "[DEBUG] Dynamic Preset: " << context.dyn_preset << "\n";
    std::cout << "[DEBUG] Slur applied: " << (phrase.slur ? "Yes" : "No") << "\n";

    float current_time = 0.0f;

    for (size_t i = 0; i < phrase.notes.size(); ++i)
    {
        const auto& note = phrase.notes[i];
        int midi = pitchToMidi(note.pitch);

        std::cout << "Note " << i + 1 << ": pitch=" << note.pitch
                  << " (MIDI " << midi << "), duration=" << note.duration_beats
                  << ", starts at beat " << current_time
                  << ", articulation=" << note.articulation
                  << ", accent=" << note.accent << "\n";

        current_time += note.duration_beats;
    }

    std::cout << "[INFO] (Stub) Would write MIDI + envelope CSV to: "
              << options.output_dir << "/" << options.output_id << ".mid/.csv\n";
}

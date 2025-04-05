#pragma once
#include <string>

struct MidiArgs {
    int note_number = 60;         // Middle C by default
    float position_beats = 0.0f;
    float duration_beats = 1.0f;
    std::string dyn_start = "mf";
    std::string dyn_end = "mf";
    std::string dyn_preset = "default";
    int controller_cc = 2;        // Default to CC2 (breath)
    std::string output_file = "output.mid";
};

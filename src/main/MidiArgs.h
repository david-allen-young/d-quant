#pragma once
#include <string>

enum class MidiController : int
{
    Modulation = 1,
    Breath = 2,
    Expression = 11
};

struct MidiArgs {
    int note_number = 60;
    float position_beats = 0.0f;
    float duration_beats = 1.0f;
    std::string dyn_start = "mf";
    std::string dyn_end = "mf";
    std::string dyn_preset = "default";
    MidiController controller_cc = MidiController::Modulation;
    std::string output_file = "output.mid";
};

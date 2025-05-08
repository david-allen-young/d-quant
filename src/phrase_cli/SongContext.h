#pragma once
#include <string>

struct SongContext {
    std::string dyn_preset = "pp_to_ff";  // range mapping preset
    float tempo_bpm = 120.0f;             // global tempo
    std::string time_signature = "4/4";   // symbolic only (not parsed)
};

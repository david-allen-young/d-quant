#pragma once
#include <string>

struct NoteArgs {
    std::string pitch;          // e.g., "C4"
    float duration_beats = 1.0; // in beats, e.g., 1.0 = quarter note at 4/4
    std::string articulation = "ordinario"; // "staccato", "tenuto", etc.
    std::string accent = "none";            // "none", "standard", "strong"
};

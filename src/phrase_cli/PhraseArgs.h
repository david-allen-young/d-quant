#pragma once
#include "NoteArgs.h"
#include <vector>
#include <string>

struct PhraseArgs {
    bool slur = false;                    // true if notes are under a phrase slur
    std::string dyn_start = "mf";         // start dynamic mark
    std::string dyn_end = "mf";           // end dynamic mark
    std::vector<NoteArgs> notes;          // ordered list of symbolic notes
};

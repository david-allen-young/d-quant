#pragma once

#include "Note.h"
#include <string>
#include <vector>

namespace rhythmizer {
//-------------------------------------------------------------------------------------------

void writeRhythmDeviationCSV(const std::vector<NoteData>& notes, const std::string& filename);

//-------------------------------------------------------------------------------------------
}//namespace rhythmizer

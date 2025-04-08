#pragma once
#include "NoteBuilder.h"
#include <string>

namespace rhythmizer {
//-------------------------------------------------------------------------------------------

void applyTiming(NoteBuilderMidi& builder, double nominalPosition, double nominalDuration, std::string articulation);

//-------------------------------------------------------------------------------------------
}//namespace rhythmizer

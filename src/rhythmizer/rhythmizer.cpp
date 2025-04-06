#include "rhythmizer.h"

namespace rhythmizer {
//-------------------------------------------------------------------------------------------

double getPositionOffset()
{
    // TODO: derive from standard distribution of offsets once input data is available
    return 0.0;
}

double getDurationRatio()
{
    // TODO: derive from standard distribution of ratios once input data is available
    return 1.0;
}

void applyTiming(NoteBuilderMidi& builder, double nominalPosition, double nominalDuration)
{
    // TODO: use nominalPosition and nominalDuration to compute expressive timing
    // e.g., builder.setPosition(nominalPosition + someOffset);
    double positionOffset = getPositionOffset();
    double durationRatio = getDurationRatio();
    builder.setPosition(nominalPosition + positionOffset);
    builder.setDuration(nominalDuration * durationRatio);
}

//-------------------------------------------------------------------------------------------
}//namespace rhythmizer

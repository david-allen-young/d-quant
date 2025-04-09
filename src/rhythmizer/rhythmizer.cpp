#include "rhythmizer.h"
#include <random>

namespace rhythmizer {
//-------------------------------------------------------------------------------------------

class RhythmVariationGenerator
{
public:
    RhythmVariationGenerator(double meanPos, double stdPos,
                                double meanDur, double stdDur,
                                unsigned int seed = std::random_device{}())
        : meanPos(meanPos), stdPos(stdPos),
            meanDur(meanDur), stdDur(stdDur),
            rng(seed)
    {
    }

    double triangular(double a, double c, double b)
    {
        std::uniform_real_distribution<> dist(0.0, 1.0);
        double u = dist(rng);
        if (u < (c - a) / (b - a))
        {
            return a + std::sqrt(u * (b - a) * (c - a));
        }
        else
        {
            return b - std::sqrt((1 - u) * (b - a) * (b - c));
        }
    }

    double getDeltaPosition()
    {
        return meanPos + stdPos * triangular(-1.0, 0.0, 1.0);
    }

    double getDurationRatio()
    {
        return meanDur + stdDur * triangular(-1.0, 0.0, 1.0);
    }

private:
    double meanPos, stdPos;
    double meanDur, stdDur;
    std::mt19937 rng;
};

RhythmVariationGenerator generator(
    /*meanPos=*/-0.0782, /*stdPos=*/0.0371,
    /*meanDur=*/0.7372, /*stdDur=*/0.0681
    // VelocityDelta is handled elsewhere, so omitted here
);

double getPositionOffset()
{
    return generator.getDeltaPosition();
}

double getDurationRatio()
{
    return generator.getDurationRatio();
}

void applyTiming(NoteBuilderMidi& builder, double nominalPosition, double nominalDuration, std::string articulation)
{
    double positionOffset = getPositionOffset();
    double position = nominalPosition + positionOffset;
    if (articulation == "staccato")
    {
        nominalDuration *= 0.6;
    }
    double duration;
    double durationRatio = getDurationRatio();
    if (nominalDuration > 1.0)
    {
        double trim = articulation == "tenuto" ? 0.1 : 1.0;
        duration = (nominalDuration - trim) + (trim * durationRatio);
    }
    else
    {
        if (articulation == "tenuto")
        {
            durationRatio = (durationRatio + 1.0) / 2.0;
        }
        duration = nominalDuration * durationRatio;
    }
    builder.setPosition(position);
    builder.setDuration(duration);
}

//-------------------------------------------------------------------------------------------
}//namespace rhythmizer

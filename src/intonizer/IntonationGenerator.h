#pragma once
#include "Point.h"
#include <vector>

namespace intonizer
{

enum class OutputUnit
{
    Cents,
    PitchWheelUnits
};

std::vector<Point> applyPitchEnvelope(
    const std::vector<std::pair<double, int>>& breathEnvelope,
    bool isStableDynamics,
    double pitchRangeInCents,
    double compensationFactor,
    OutputUnit unit = OutputUnit::PitchWheelUnits);

} // namespace intonizer

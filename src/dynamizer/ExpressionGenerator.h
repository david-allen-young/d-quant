#pragma once
#include "Point.h"
#include "ExpressionMarks.h"
#include <vector>

namespace dynamizer {
//-------------------------------------------------------------------------------------------

std::vector<std::pair<double, int>> generateBreathCCFromEnvelope(
    const std::vector<Point>& envelope,
    double durationInBeats,
    ExpressionMark startDynamic,
    ExpressionMark endDynamic,
    ExpressionMark minDynamicInScore,
    ExpressionMark maxDynamicInScore);

//-------------------------------------------------------------------------------------------
}//namespace dynamizer

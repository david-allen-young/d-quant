#pragma once
#include "Point.h"
#include "ExpressionMarks.h"
#include <vector>
using std::vector;
using std::pair;

namespace dynamizer {
//-------------------------------------------------------------------------------------------

vector<pair<double, int>> generateBreathCCFromEnvelope(
    const vector<Point>& envelope,
    double durationInBeats,
    ExpressionMark startDynamic,
    ExpressionMark endDynamic,
    ExpressionMark minDynamicInScore,
    ExpressionMark maxDynamicInScore);

//-------------------------------------------------------------------------------------------
}//namespace dynamizer

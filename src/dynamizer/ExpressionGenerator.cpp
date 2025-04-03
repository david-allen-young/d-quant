#include "ExpressionGenerator.h"

std::vector<std::pair<double, int>> generateBreathCCFromEnvelope(
    const std::vector<Point>& envelope,
    double durationInBeats,
    ExpressionMark startDynamic,
    ExpressionMark endDynamic,
    ExpressionMark minDynamicInScore,
    ExpressionMark maxDynamicInScore)
{
    std::vector<std::pair<double, int>> result;

    int minVal = static_cast<int>(minDynamicInScore);
    int maxVal = static_cast<int>(maxDynamicInScore);
    int startVal = static_cast<int>(startDynamic);
    int endVal = static_cast<int>(endDynamic);

    if (startVal == endVal)
    {
        // Single dynamic level: one CC event at the start
        int ccVal = expressionMarkToCC(startDynamic, minDynamicInScore, maxDynamicInScore);
        result.emplace_back(0.0, ccVal);
        return result;
    }

    // Map each envelope point to a scaled value across the dynamic range
    int lastCC = -1;
    for (const auto& point : envelope)
    {
        double alpha = point.value; // normalized [0–1] envelope
        double dynamicVal = startVal + alpha * (endVal - startVal);
        double norm = (dynamicVal - minVal) / (maxVal - minVal);
        int ccVal = static_cast<int>(norm * 127.0);
        if (ccVal == lastCC)
        {
            continue;
        }
        lastCC = ccVal;
        double positionInBeats = point.time * durationInBeats;
        result.emplace_back(positionInBeats, ccVal);
    }

    return result;
}

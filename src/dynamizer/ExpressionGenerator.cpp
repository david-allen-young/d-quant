#include "ExpressionGenerator.h"
#include "ExpressionMarks.h" // for expressionMarkToCC
#include "EnvelopeUtils.h"
#include <algorithm>
#include <cmath>

namespace dynamizer
{
//-------------------------------------------------------------------------------------------

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
        minVal -= 1;
        double rangeMark = static_cast<double>(maxVal - minVal) * 1.01;
        double normalizedMark = (startVal - minVal) / rangeMark;
        for (const auto& pt : envelope)
        {
            double envelopeCenter = computeEnvelopeMean(envelope);
            double delta = pt.value - envelopeCenter;
            double dynamicVal = normalizedMark + delta;
            dynamicVal = std::clamp(dynamicVal, 0.0, 1.0);
            int ccVal = static_cast<int>(dynamicVal * 126.0);
            double positionInBeats = pt.time * durationInBeats;
            if (!result.empty() && result.back().second == ccVal)
            {
                continue;
            }
            result.emplace_back(positionInBeats, ccVal);
        }
        return result;
    }

    std::vector<Point> adjustedEnvelope = envelope;
    if (startVal > endVal)
    {
        for (auto& pt : adjustedEnvelope)
        {
            pt.value = 1.0 - pt.value;
        }
    }

    int lastCC = -1;
    for (const auto& point : adjustedEnvelope)
    {
        double alpha = point.value;
        double dynamicVal = startVal + alpha * (endVal - startVal);
        dynamicVal = std::clamp(dynamicVal, static_cast<double>(minVal), static_cast<double>(maxVal));

        ExpressionMark dynMark = static_cast<ExpressionMark>(static_cast<int>(std::round(dynamicVal)));
        int ccVal = expressionMarkToCC(dynMark, minDynamicInScore, maxDynamicInScore);

        if (ccVal == lastCC)
        {
            continue;
        }
        lastCC = ccVal;
        double positionInBeats = point.time * durationInBeats;
        if (!result.empty() && std::abs(positionInBeats - result.back().first) < 0.01)
        {
            result.back().second = ccVal;
        }
        else
        {
            result.emplace_back(positionInBeats, ccVal);
        }
    }

    return result;
}

//-------------------------------------------------------------------------------------------
} // namespace dynamizer

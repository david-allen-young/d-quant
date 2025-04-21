#include "ExpressionGenerator.h"
#include "ExpressionMarks.h" // for expressionMarkToCC
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
        // Find min/max of envelope values
        double minEnv = envelope.front().value;
        double maxEnv = envelope.front().value;
        for (const auto& pt : envelope)
        {
            minEnv = std::min(minEnv, pt.value);
            maxEnv = std::max(maxEnv, pt.value);
        }

        double range = maxEnv - minEnv;
        if (range < 1e-5)
            range = 1.0; // fallback to avoid divide-by-zero

        for (const auto& point : envelope)
        {
            // Normalize to [0.0, 1.0]
            double normalized = (point.value - minEnv) / range;

            double fluctuationRange = (maxVal - minVal) * 0.3;
            double dynamicVal = static_cast<double>(startVal) + (normalized - 0.5) * fluctuationRange;
            dynamicVal = std::clamp(dynamicVal, static_cast<double>(minVal), static_cast<double>(maxVal));

            double norm = (dynamicVal - minVal) / (maxVal - minVal);
            int ccVal = static_cast<int>(norm * 127.0); // KEEP THIS

            double positionInBeats = point.time * durationInBeats;
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

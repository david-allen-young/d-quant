#include "ExpressionGenerator.h"
#include <algorithm>

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

            //// Use normalized to interpolate around startVal
            //double dynamicVal = static_cast<double>(startVal) + (normalized - 0.5) * (maxVal - minVal) * 0.5;

            double fluctuationRange = (maxVal - minVal) * 0.3;
            double dynamicVal = static_cast<double>(startVal) + (normalized - 0.5) * fluctuationRange;

            // Clamp to dynamic range
            dynamicVal = std::clamp(dynamicVal, static_cast<double>(minVal), static_cast<double>(maxVal));

            double norm = (dynamicVal - minVal) / (maxVal - minVal);
            int ccVal = static_cast<int>(norm * 127.0);

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

    // Map each envelope point to a scaled value across the dynamic range
    int lastCC = -1;
    for (const auto& point : /*envelope*/adjustedEnvelope)
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
}// namespace dynamizer
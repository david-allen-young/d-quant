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
        // Calculate center of the envelope
        double envMean = 0.0;
        for (const auto& pt : envelope)
            envMean += pt.value;
        envMean /= static_cast<double>(envelope.size());

        // Safety check to avoid divide by zero
        double epsilon = 1e-6;
        double scale = (maxVal - minVal) > epsilon ? 1.0 / (envMean + epsilon) : 1.0;

        for (const auto& point : envelope)
        {
            // Scale and offset to match desired center
            double adjusted = point.value * scale;

            // Clamp to [0.0, 1.0]
            adjusted = std::clamp(adjusted, 0.0, 1.0);

            // Map to dynamic range
            double dynamicVal = minVal + adjusted * (maxVal - minVal);

            // Clamp again to make sure we stay inside bounds
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
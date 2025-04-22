#include "IntonationGenerator.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>

namespace intonizer
{

namespace
{
constexpr double kMidiPitchWheelRange = 8192.0;

double computeEnvelopeMean(const std::vector<Point>& envelope)
{
    if (envelope.empty())
        return 0.0;

    double sum = std::accumulate(envelope.begin(), envelope.end(), 0.0,
                                 [](double acc, const Point& pt)
                                 { return acc + pt.value; });
    return sum / static_cast<double>(envelope.size());
}

int computeCCMean(const std::vector<std::pair<double, int>>& ccData)
{
    if (ccData.empty())
    {
        return 0;
    }
    int sum = std::accumulate(ccData.begin(), ccData.end(), 0,
                              [](int acc, const std::pair<double, int>& pt)
                              { return acc + pt.second; });
    return sum / ccData.size();
}

int centsToPitchWheel(double cents)
{
    // Assume +/-100 cents maps to +/-8192 units
    double scaled = (cents / 100.0) * kMidiPitchWheelRange;
    return std::clamp(static_cast<int>(std::round(scaled)), -8192, 8191);
}
} // namespace

std::vector<Point> applyPitchEnvelope(
    const std::vector<std::pair<double, int>>& breathEnvelope,
    bool isStableDynamics,
    double centsPerDeltaCC,
    double compensationFactor,
    OutputUnit unit)
{
    std::vector<Point> result;
    result.reserve(breathEnvelope.size());
    std::cout << "breathCC.size()==" << breathEnvelope.size() << std::endl;

    if (/*isStableDynamics*/true)
    {
        //double envelopeCenter = computeEnvelopeMean(breathEnvelope);

        //std::cout << "[DEBUG] Stable envelope center: " << envelopeCenter << std::endl;

        //for (const auto& pt : breathEnvelope)
        //{
        //    double delta = pt.value - envelopeCenter;

        //    // Apply scaling conservatively
        //    double cents = delta * pitchRangeInCents;

        //    // Clamp soft outliers
        //    cents = std::clamp(cents, -pitchRangeInCents, pitchRangeInCents);

        //    double value = (unit == OutputUnit::PitchWheelUnits)
        //                       ? static_cast<double>(centsToPitchWheel(cents))
        //                       : cents;

        //    result.emplace_back(pt.time, value);
        //}
        int breathCenter = computeCCMean(breathEnvelope);
        std::cout << "[DEBUG] Stable breath CC center: " << breathCenter << std::endl;
        for (const auto& pt : breathEnvelope)
        {
            int delta = pt.second - breathCenter;
            //centsPerDeltaCC *= compensationFactor;
            double cents = static_cast<double>(delta) * centsPerDeltaCC;
            //cents = std::clamp(cents, -25.0, 25.0);
            
            //cents /= 2;
            cents *= compensationFactor;

            //double value = (unit == OutputUnit::PitchWheelUnits)
            //                    ? static_cast<double>(centsToPitchWheel(cents))
            //                    : cents;
            double value = static_cast<double>(centsToPitchWheel(cents));
            std::cout << "delta==" << delta << "; cents==" << cents << "; value==" << value << std::endl;
            result.emplace_back(pt.first, value);
        }

        std::cout << "[DEBUG] Final pitch envelope (to be added to builder):\n";
        for (const auto& pt : result)
        {
            std::cout << "  beat " << pt.time << " -> PB " << pt.value << std::endl;
        }

        return result;
    }
    else
    {
        std::vector<Point> normalizedEnvelope;
        for (auto& pt : breathEnvelope) // currently CC values 0 to 127
        {
            double time = pt.first;
            int value = pt.second;
            double norm = value / 127.0;
            normalizedEnvelope.emplace_back(time, norm);
        }

        constexpr double center = 0.5;

        for (const auto& pt : normalizedEnvelope)
        {
            double norm = std::clamp(pt.value, 0.0, 1.0);
            double offset = (norm - center) * centsPerDeltaCC;
            double cents = offset * compensationFactor;

            double value = (unit == OutputUnit::PitchWheelUnits)
                               ? static_cast<double>(centsToPitchWheel(cents))
                               : cents;

            result.emplace_back(pt.time, value);
        }
    }

    return result;
}

} // namespace intonizer

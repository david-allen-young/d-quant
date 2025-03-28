#include "EnvelopeUtils.h"
#include <algorithm>

constexpr double safe_div_epsilon = 1e-8;

std::vector<double> interpolateEnvelope(const std::vector<Point>& envelope, size_t targetLength)
{
    std::vector<double> result(targetLength);
    if (envelope.empty())
    {
        return result;
    }

    double tMin = envelope.front().time;
    double tMax = envelope.back().time;
    std::vector<double> targetTime(targetLength);
    for (size_t i = 0; i < targetLength; ++i)
    {
        targetTime[i] = tMin + (tMax - tMin) * i / (targetLength - 1);
    }

    size_t j = 0;
    for (size_t i = 0; i < targetLength; ++i)
    {
        double t = targetTime[i];
        while (j + 1 < envelope.size() && envelope[j + 1].time < t)
        {
            ++j;
        }

        if (j + 1 >= envelope.size())
        {
            result[i] = envelope.back().value;
        }
        else
        {
            double t0 = envelope[j].time;
            double t1 = envelope[j + 1].time;
            double v0 = envelope[j].value;
            double v1 = envelope[j + 1].value;

            //double alpha = (t - t0) / (t1 - t0 + 1e-8);
            double alpha = (t - t0) / ((t1 - t0) + safe_div_epsilon);
            result[i] = v0 + alpha * (v1 - v0);
        }
    }

    return result;
}

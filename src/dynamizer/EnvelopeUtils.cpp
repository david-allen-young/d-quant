#include "EnvelopeUtils.h"
#include <algorithm>
#include <cmath>

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

void analyzeEnvelopes(const std::vector<std::vector<double>>& envelopes, std::vector<double>& outMean, std::vector<double>& outStd)
{
    if (envelopes.empty())
    {
        return;
    }

    size_t numSamples = envelopes[0].size();
    size_t numEnvelopes = envelopes.size();

    outMean.assign(numSamples, 0.0);
    outStd.assign(numSamples, 0.0);

    // Compute mean
    for (const auto& env : envelopes)
    {
        for (size_t i = 0; i < numSamples; ++i)
        {
            outMean[i] += env[i];
        }
    }

    for (double& val : outMean)
    {
        val /= static_cast<double>(numEnvelopes);
    }

    // Compute std dev
    for (const auto& env : envelopes)
    {
        for (size_t i = 0; i < numSamples; ++i)
        {
            double diff = env[i] - outMean[i];
            outStd[i] += diff * diff;
        }
    }

    for (double& val : outStd)
    {
        val = std::sqrt(val / static_cast<double>(numEnvelopes));
    }
}

std::vector<double> softClipDelta(const std::vector<double>& delta, const std::vector<double>& std, double softness = 3.0)
{
    std::vector<double> result(delta.size());
    for (size_t i = 0; i < delta.size(); ++i)
    {
        double denom = std[i] + safe_div_epsilon;
        result[i] = std[i] * std::tanh((delta[i] / denom) * softness);
    }
    return result;
}

std::vector<double> morphFromTwoInputs(const Morph2Params& p)
{
    size_t N = p.mean.size();
    std::vector<double> result(N);
    std::vector<double> trend(N);
    std::vector<double> delta(N);
    std::vector<double> adaptiveMorph(N);

    double morphFactor = 0.5 * (p.morphFactor1 + p.morphFactor2);
    double maxStd = *std::max_element(p.std.begin(), p.std.end()) + safe_div_epsilon;

    for (size_t i = 0; i < N; ++i)
    {
        trend[i] = 0.5 * (p.input1[i] + p.input2[i]);
        delta[i] = trend[i] - p.mean[i];
        adaptiveMorph[i] = morphFactor * (p.std[i] / maxStd);
    }

    std::vector<double> morphed(N);
    for (size_t i = 0; i < N; ++i)
    {
        morphed[i] = p.mean[i] + adaptiveMorph[i] * delta[i];
    }

    std::vector<double> softClamped(N);
    for (size_t i = 0; i < N; ++i)
    {
        double d = morphed[i] - p.mean[i];
        double denom = p.std[i] + safe_div_epsilon;
        softClamped[i] = p.std[i] * std::tanh((d / denom) * p.softness);
    }

    for (size_t i = 0; i < N; ++i)
    {
        result[i] = std::clamp(p.mean[i] + softClamped[i], 0.0, 1.0);
    }

    return result;
}

double computeEnvelopeMean(const std::vector<Point>& envelope)
{
    if (envelope.empty())
        return 0.0;
    double sum = 0.0;
    for (const auto& pt : envelope)
        sum += pt.value;
    return sum / static_cast<double>(envelope.size());
}

std::vector<Point> extractEnvelopeSegment(const std::vector<Point>& envelope, double noteStart, double noteEnd)
{
    std::vector<Point> result;
    for (const auto& pt : envelope)
    {
        if (pt.time >= noteStart && pt.time < noteEnd)
        {
            //result.emplace_back(pt.time - noteStart, pt.value); // relative time
            result.emplace_back(pt.time, pt.value); // absolute time
        }
    }
    return result;
}

std::vector<std::pair<double, int>> extractEnvelopeSegment(const std::vector<std::pair<double, int>>& envelope, double noteStart, double noteEnd)
{
    std::vector<std::pair<double, int>> result;
    for (const auto& [time, value] : envelope)
    {
        if (time >= noteStart && time < noteEnd)
        {
            //result.emplace_back(time - noteStart, value); // relative time
            result.emplace_back(time, value); // absolute time
        }
    }
    return result;
}
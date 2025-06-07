#pragma once

#include "Point.h" // from core module
#include <vector>

struct Morph2Params
{
        const std::vector<double>& mean;
        const std::vector<double>& input1;
        const std::vector<double>& input2;
        const std::vector<double>& std;
        double morphFactor1 = 0.5;
        double morphFactor2 = 0.5;
        double softness = 3.0;
};

std::vector<double> interpolateEnvelope(const std::vector<Point>& envelope, size_t targetLength = 100);
void analyzeEnvelopes(const std::vector<std::vector<double>>& envelopes, std::vector<double>& outMean, std::vector<double>& outStd);
std::vector<double> morphFromTwoInputs(const Morph2Params& params);
double computeEnvelopeMean(const std::vector<Point>& envelope);
std::vector<Point> extractEnvelopeSegment(const std::vector<Point>& envelope, double noteStart, double noteEnd);

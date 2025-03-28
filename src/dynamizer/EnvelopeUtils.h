#pragma once

#include "Point.h" // from core module
#include <vector>

std::vector<double> interpolateEnvelope(const std::vector<Point>& envelope, size_t targetLength = 100);
void analyzeEnvelopes(const std::vector<std::vector<double>>& envelopes, std::vector<double>& outMean, std::vector<double>& outStd);

#pragma once

#include "Point.h" // from core module
#include <vector>

std::vector<double> interpolateEnvelope(const std::vector<Point>& envelope, size_t targetLength = 100);

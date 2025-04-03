#pragma once
#include "Point.h"
#include <vector>
#include <string>

double linearInterpolate(double time, const std::vector<Point>& envelope);
std::vector<double> mergeTimelines(const std::vector<std::vector<Point>>& envelopes);
std::vector<Point> computeMeanEnvelope(const std::vector<std::vector<Point>>& envelopes);
void writeCSV(const std::vector<Point>& envelope, const std::string& filename);
bool readCSV(const std::string& filename, std::vector<Point>& outEnvelope);

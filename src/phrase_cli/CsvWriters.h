#pragma once
#include "Point.h"
#include <vector>
#include <string>
#include <utility>

void writeEnvelopeCsv(const std::string& outPath, const std::vector<Point>& ccPoints);
void writeEnvelopeCsv(const std::string& outPath, const std::vector<std::pair<double, double>>& ccPoints);
void writeRhythmizerCsv(const std::string& outPath, const std::vector<std::tuple<double, double, double>>& deltas);

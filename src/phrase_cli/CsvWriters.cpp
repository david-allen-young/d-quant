#include "CsvWriters.h"
#include <fstream>
#include <stdexcept>

void writeEnvelopeCsv(const std::string& outPath, const std::vector<std::pair<double, double>>& ccPoints)
{
    std::ofstream file(outPath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open envelope CSV for writing: " + outPath);

    file << "Position,Expression\n";
    for (const auto& [pos, val] : ccPoints)
    {
        file << pos << "," << val << "\n";
    }
}

void writeRhythmizerCsv(const std::string& outPath, const std::vector<std::tuple<double, double, double>>& deltas)
{
    std::ofstream file(outPath);
    if (!file.is_open())
        throw std::runtime_error("Cannot open rhythmizer CSV for writing: " + outPath);

    file << "NominalPosition,DeltaPosition,DurationRatio\n";
    for (const auto& [nominal, delta, ratio] : deltas)
    {
        file << nominal << "," << delta << "," << ratio << "\n";
    }
}

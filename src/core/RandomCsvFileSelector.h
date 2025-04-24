#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <sstream>

std::mt19937& globalRng()
{
    static std::mt19937 rng(std::random_device{}());
    return rng;
}

std::string selectRandomCsvInDir(const std::string& dirPath)
{
    std::vector<std::string> csvFiles;
    for (const auto& entry : std::filesystem::directory_iterator(dirPath))
    {
        if (entry.path().extension() == ".csv")
        {
            csvFiles.push_back(entry.path().string());
        }
    }

    if (csvFiles.empty())
    {
        throw std::runtime_error("No CSV files found in directory: " + dirPath);
    }

    std::uniform_int_distribution<> indexDist(0, static_cast<int>(csvFiles.size()) - 1);
    int rIdx = indexDist(globalRng());
    //std::cout << "[DEBUG] Total morphs available: " << csvFiles.size() << std::endl;
    //std::cout << "[DEBUG] Selected morph envelope: " << csvFiles[rIdx] << std::endl;

    return csvFiles[rIdx];
}
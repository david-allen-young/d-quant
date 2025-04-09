#include "AnalysisHelpers.h"
#include "ExpressionGenerator.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>
#include <random>

using namespace dynamizer;
namespace fs = std::filesystem;

TEST_CASE("run_breath_cc_generation produces valid CC data", "[regression]")
{
    const std::string inputDir = "test_output/morphs"; // from previous test
    const std::string outputDir = "test_output/breath_cc";

    fs::create_directories(outputDir);
    std::vector<std::vector<Point>> morphedEnvelopes;

    for (const auto& entry : fs::directory_iterator(inputDir))
    {
        if (entry.path().extension() == ".csv")
        {
            std::vector<Point> points;
            readCSV(entry.path().string(), points);
            if (!points.empty())
                morphedEnvelopes.push_back(points);
        }
    }

    REQUIRE(!morphedEnvelopes.empty());

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> indexDist(0, static_cast<int>(morphedEnvelopes.size() - 1));
    auto envelope = morphedEnvelopes[indexDist(rng)];

    double durationBeats = 4.0;
    auto result = generateBreathCCFromEnvelope(
        envelope,
        durationBeats,
        ExpressionMark::pp,
        ExpressionMark::ff,
        ExpressionMark::pp,
        ExpressionMark::ff);

    std::vector<Point> output;
    for (const auto& [beat, cc] : result)
    {
        CHECK(cc >= 0);
        CHECK(cc <= 127);
        output.push_back({beat, static_cast<double>(cc)});
    }

    const std::string outFile = outputDir + "/breath_cc_test.csv";
    writeCSV(output, outFile);
    std::cout << "Wrote: " << fs::absolute(outFile) << std::endl;

    CHECK(!output.empty());
}

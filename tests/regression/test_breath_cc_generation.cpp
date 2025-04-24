#include "AnalysisHelpers.h"
#include "ExpressionGenerator.h"
#include "test_data_utils.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>
#include <random>

#include "PipelineArgsRegistry.h"
using namespace dynamizer;
namespace fs = std::filesystem;

#include "core/PathRegistry.h"

TEST_CASE("run_breath_cc_generation produces valid CC data", "[regression]")
{
    ensure_morphs_exist();
    const auto& args = getPipelineArgs();

    //const std::string inputDir = args.morph_csv_dir;
    //const std::string outputDir = args.output_dir + "/breath_cc";

    const auto testsBase = PathRegistry::getResolvedPath("working_dir_tests");
    const auto inputDir = (testsBase / "morphs_csv").lexically_normal();
    const auto outputDir = (testsBase / "breath_cc").lexically_normal();

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

    const std::string outFile = (outputDir / "breath_cc_test.csv").lexically_normal().string();
    writeCSV(output, outFile);
    std::cout << "Wrote: " << fs::absolute(outFile) << std::endl;

    CHECK(!output.empty());
}

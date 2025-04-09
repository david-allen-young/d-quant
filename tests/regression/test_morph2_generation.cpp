#include "AnalysisHelpers.h"
#include "dynamizer.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

TEST_CASE("run_morph2_generation_example generates valid envelopes", "[regression]")
{
    const std::string inputDir = "test_output/envelopes"; // From previous test
    const std::string outputDir = "test_output/morphs";
    const int count = 10;

    fs::create_directories(outputDir);
    std::cout << "[TEST] Generating morphs in: " << fs::absolute(outputDir) << "\n";

    run_morph2_generation_example(inputDir, outputDir, count);

    size_t validCount = 0;
    for (const auto& entry : fs::directory_iterator(outputDir))
    {
        if (entry.path().extension() == ".csv")
        {
            std::cout << "[OUT] " << fs::absolute(entry.path()) << "\n";

            std::vector<Point> envelope;
            readCSV(entry.path().string(), envelope);
            if (!envelope.empty())
            {
                for (const auto& p : envelope)
                {
                    CHECK(p.value >= 0.0);
                    CHECK(p.value <= 1.0);
                }
                ++validCount;
            }
        }
    }

    CHECK(validCount == count);
}

#include "AnalysisHelpers.h"
#include "dynamizer.h"
#include "test_data_utils.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>

#include "PipelineArgs.h"
namespace fs = std::filesystem;

#include "core/PathRegistry.h"

TEST_CASE("run_morph2_generation_example generates valid envelopes", "[regression]")
{
    ensure_envelopes_exist();
    const auto& args = getPipelineArgs();
    
    //const std::string inputDir = args.envelope_csv_dir;
    //const std::string outputDir = args.morph_csv_dir;

    const auto testsBase = PathRegistry::getResolvedPath("working_dir_tests");
    const auto inputDir = (testsBase / "midi_to_csv").lexically_normal();
    const auto outputDir = (testsBase / "morphs_csv").lexically_normal();

    const int count = 10;

    fs::create_directories(outputDir);
    std::cout << "[TEST] Generating morphs in: " << fs::absolute(outputDir) << "\n";

    run_morph2_generation_example(inputDir.string(), outputDir.string(), count);

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

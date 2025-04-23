#include "dynamizer.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "PipelineArgsRegistry.h"
namespace fs = std::filesystem;

#include "core/PathRegistry.h"

TEST_CASE("run_midi_to_envelope_csv outputs expected CSV files", "[regression]")
{
    const auto& args = getPipelineArgs();
    //const std::string inputMidi = args.dynamizer_midi_path;
    //const std::string outDir = args.envelope_csv_dir;

    const auto inputBase = PathRegistry::getResolvedPath("input_midi_dir");
    const auto inputDir = (inputBase / "").lexically_normal();
    const auto inputPath = (inputDir / "Dynamizer_Training_CRE_pp_to_ff_0001.midi").lexically_normal();

    const auto testsBase = PathRegistry::getResolvedPath("working_dir_tests");
    const auto outputDir = (testsBase / "midi_to_csv").lexically_normal();

    fs::create_directories(outputDir);
    std::cout << "Writing envelope CSVs to: " << fs::absolute(outputDir) << "\n";

    run_midi_to_envelope_csv(inputPath.string(), outputDir.string());

    size_t csvCount = 0;
    for (const auto& entry : fs::directory_iterator(outputDir))
    {
        if (entry.path().extension() == ".csv")
        {
            std::cout << "Found: " << entry.path().filename() << "\n";
            ++csvCount;
        }
    }

    CHECK(csvCount > 0);
}

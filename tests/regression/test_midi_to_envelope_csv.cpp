#include "dynamizer.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "PipelineArgsRegistry.h"
namespace fs = std::filesystem;

TEST_CASE("run_midi_to_envelope_csv outputs expected CSV files", "[regression]")
{
    const auto& args = getPipelineArgs();
    const std::string inputMidi = args.dynamizer_midi_path;
    const std::string outDir = args.envelope_csv_dir;

    fs::create_directories(outDir);
    std::cout << "Writing envelope CSVs to: " << fs::absolute(outDir) << "\n";

    run_midi_to_envelope_csv(inputMidi, outDir);

    size_t csvCount = 0;
    for (const auto& entry : fs::directory_iterator(outDir))
    {
        if (entry.path().extension() == ".csv")
        {
            std::cout << "Found: " << entry.path().filename() << "\n";
            ++csvCount;
        }
    }

    CHECK(csvCount > 0);
}

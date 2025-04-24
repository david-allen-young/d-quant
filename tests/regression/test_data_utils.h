#pragma once

#include "dynamizer.h"
#include <filesystem>
#include <iostream>

#include "PipelineArgsRegistry.h"
namespace fs = std::filesystem;

#include "core/PathRegistry.h"

inline void ensure_envelopes_exist()
{
    const auto& args = getPipelineArgs();

    //const std::string inputMidi = args.dynamizer_midi_path;
    //const std::string envelopeDir = args.envelope_csv_dir;

    const auto inputBase = PathRegistry::getResolvedPath("dynamizer_training");
    const auto inputDir = (inputBase / "").lexically_normal();
    const auto inputPath = (inputDir / "Dynamizer_Training_CRE_pp_to_ff_0001.midi").lexically_normal();

    const auto testsBase = PathRegistry::getResolvedPath("working_dir_tests");
    const auto envelopesDir = (testsBase / "midi_to_csv").lexically_normal();

    if (!fs::exists(inputDir) || fs::is_empty(inputDir))
    {
        std::cout << "[SETUP] Generating envelopes from MIDI: " << inputPath << "\n";
        fs::create_directories(envelopesDir);
        run_midi_to_envelope_csv(inputPath.string(), envelopesDir.string());
    }
}

inline void ensure_morphs_exist()
{
    ensure_envelopes_exist(); // morphs depend on envelope input
    
    //const auto& args = getPipelineArgs();
    //const std::string morphDir = args.morph_csv_dir;

    const auto testsBase = PathRegistry::getResolvedPath("working_dir_tests");
    const auto morphsDir = (testsBase / "morphs_csv").lexically_normal();

    if (!fs::exists(morphsDir) || fs::is_empty(morphsDir))
    {
        std::cout << "[SETUP] Generating morphs from envelopes...\n";
        run_morph2_generation_example("test_output/envelopes", morphsDir.string(), 10);
    }
}

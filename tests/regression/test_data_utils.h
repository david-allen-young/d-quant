#pragma once

#include "dynamizer.h"
#include <filesystem>
#include <iostream>

#include "PipelineArgsRegistry.h"
namespace fs = std::filesystem;

inline void ensure_envelopes_exist()
{
    const auto& args = getPipelineArgs();
    const std::string inputMidi = args.dynamizer_midi_path;
    const std::string envelopeDir = args.envelope_csv_dir;

    if (!fs::exists(envelopeDir) || fs::is_empty(envelopeDir))
    {
        std::cout << "[SETUP] Generating envelopes from MIDI: " << fs::absolute(inputMidi) << "\n";
        fs::create_directories(envelopeDir);
        run_midi_to_envelope_csv(inputMidi, envelopeDir);
    }
}

inline void ensure_morphs_exist()
{
    ensure_envelopes_exist(); // morphs depend on envelope input
    const auto& args = getPipelineArgs();
    const std::string morphDir = args.morph_csv_dir;
    if (!fs::exists(morphDir) || fs::is_empty(morphDir))
    {
        std::cout << "[SETUP] Generating morphs from envelopes...\n";
        run_morph2_generation_example("test_output/envelopes", morphDir, 10);
    }
}

#include "PipelineConfigLoader.h"
#include "PipelineArgs.h"
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

bool load_args_from_json(const std::string& path, PipelineArgs& out_args)
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    json j;
    file >> j;

    if (j.contains("dynamizer_midi_path"))
        out_args.dynamizer_midi_path = j["dynamizer_midi_path"];
    if (j.contains("rhythmizer_midi_path"))
        out_args.rhythmizer_midi_path = j["rhythmizer_midi_path"];
    if (j.contains("envelope_csv_dir"))
        out_args.envelope_csv_dir = j["envelope_csv_dir"];
    if (j.contains("morph_csv_dir"))
        out_args.morph_csv_dir = j["morph_csv_dir"];
    if (j.contains("rhythm_deviation_csv"))
        out_args.rhythm_deviation_csv = j["rhythm_deviation_csv"];
    if (j.contains("output_dir"))
        out_args.output_dir = j["output_dir"];

    return true;
}

#include "PathUtils.h" // for env_or_fallback()
#include <iostream>

PipelineArgs getEffectivePipelineArgs(int argc, char* argv[])
{
    PipelineArgs args;

    std::string configPath;

    // CLI support for --config <file>
    for (int i = 1; i < argc - 1; ++i)
    {
        if (std::string(argv[i]) == "--config")
        {
            configPath = argv[i + 1];
            break;
        }
    }

    if (!configPath.empty())
    {
        if (!load_args_from_json(configPath, args))
        {
            std::cerr << "Warning: Failed to load config: " << configPath << "\n";
        }
    }

    // Fallbacks for anything missing
    if (args.dynamizer_midi_path.empty())
        args.dynamizer_midi_path = env_or_fallback("DQUANT_DYNAMIZER_MIDI", "assets/midi/sample_training_file_dynamizer.mid");

    if (args.rhythmizer_midi_path.empty())
        args.rhythmizer_midi_path = env_or_fallback("DQUANT_RHYTHMIZER_MIDI", "assets/midi/sample_training_file_rhythmizer.mid");

    if (args.envelope_csv_dir.empty())
        args.envelope_csv_dir = env_or_fallback("DQUANT_ENVELOPES", "test_output/envelopes");

    if (args.morph_csv_dir.empty())
        args.morph_csv_dir = env_or_fallback("DQUANT_MORPHS", "test_output/morphs");

    if (args.rhythm_deviation_csv.empty())
        args.rhythm_deviation_csv = env_or_fallback("DQUANT_RHYTHM_CSV", "test_output/rhythmizer/deviation.csv");

    if (args.output_dir.empty())
        args.output_dir = env_or_fallback("DQUANT_OUT_DIR", "test_output");

    return args;
}

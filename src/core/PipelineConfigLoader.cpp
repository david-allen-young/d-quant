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

    if (j.contains("input_midi_path"))
        out_args.input_midi_path = j["input_midi_path"];
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

#include "JsonConfigLoader.h"
#include "ControllerParser.h"
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

bool load_args_from_json(const std::string& path, MidiArgs& out_args)
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    json j;
    file >> j;

    if (j.contains("note_number"))
        out_args.note_number = j["note_number"];
    if (j.contains("position_beats"))
        out_args.position_beats = j["position_beats"];
    if (j.contains("duration_beats"))
        out_args.duration_beats = j["duration_beats"];
    if (j.contains("articulation_preset"))
        out_args.articulation_preset = j["articulation_preset"];
    if (j.contains("dyn_start"))
        out_args.dyn_start = j["dyn_start"];
    if (j.contains("dyn_end"))
        out_args.dyn_end = j["dyn_end"];
    if (j.contains("dyn_preset"))
        out_args.dyn_preset = j["dyn_preset"];
    if (j.contains("controller_cc"))
    {
        if (j["controller_cc"].is_string())
        {
            out_args.controller_cc = parseController(j["controller_cc"].get<std::string>());
        }
        else if (j["controller_cc"].is_number_integer())
        {
            out_args.controller_cc = parseController(std::to_string(j["controller_cc"].get<int>()));
        }
        else
        {
            throw std::runtime_error("controller_cc must be a number or a string");
        }
    }
    if (j.contains("output_file"))
        out_args.output_file = j["output_file"];
    if (j.contains("morph_csv_dir"))
        out_args.morph_csv_dir = j["morph_csv_dir"];
    if (j.contains("rhythm_deviation_csv"))
        out_args.rhythm_deviation_csv = j["rhythm_deviation_csv"];


    return true;
}

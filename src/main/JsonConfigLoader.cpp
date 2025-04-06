#include "JsonConfigLoader.h"
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
    if (j.contains("dyn_start"))
        out_args.dyn_start = j["dyn_start"];
    if (j.contains("dyn_end"))
        out_args.dyn_end = j["dyn_end"];
    if (j.contains("dyn_preset"))
        out_args.dyn_preset = j["dyn_preset"];
    if (j.contains("controller_cc"))
        out_args.controller_cc = j["controller_cc"];
    if (j.contains("output_file"))
        out_args.output_file = j["output_file"];

    return true;
}

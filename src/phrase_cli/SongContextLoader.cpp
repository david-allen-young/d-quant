#include "SongContextLoader.h"
#include <fstream>
#include <stdexcept>

#include <json.hpp>
using json = nlohmann::json;

bool load_song_context_from_json(const std::string& path, SongContext& out_context)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open song context JSON file: " + path);
    }

    json j;
    file >> j;

    out_context.dyn_preset = j.value("dyn_preset", "pp_to_ff");
    out_context.tempo_bpm = j.value("tempo_bpm", 120.0f);
    out_context.time_signature = j.value("time_signature", "4/4");

    return true;
}

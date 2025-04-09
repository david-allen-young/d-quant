#pragma once
#include <string>

struct PipelineArgs
{
        std::string input_midi_path;
        std::string envelope_csv_dir;
        std::string morph_csv_dir;
        std::string rhythm_deviation_csv;
        std::string output_dir;
};

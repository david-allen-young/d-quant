#pragma once
#include <string>

struct PipelineArgs
{
        std::string dynamizer_midi_path;
        std::string rhythmizer_midi_path;
        std::string envelope_csv_dir;
        std::string morph_csv_dir;
        std::string rhythm_deviation_csv;
        std::string output_dir;
};

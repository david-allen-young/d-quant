#include "CliParser.h"
#include <iostream>
#include <stdexcept>

MidiArgs parse_args(int argc, char* argv[])
{
    MidiArgs args;

    for (int i = 1; i < argc; ++i)
    {
        std::string key = argv[i];
        if (key == "--note" && i + 1 < argc)
            args.note_number = std::stoi(argv[++i]);
        else if (key == "--pos" && i + 1 < argc)
            args.position_beats = std::stof(argv[++i]);
        else if (key == "--dur" && i + 1 < argc)
            args.duration_beats = std::stof(argv[++i]);
        else if (key == "--dyn_start" && i + 1 < argc)
            args.dyn_start = argv[++i];
        else if (key == "--dyn_end" && i + 1 < argc)
            args.dyn_end = argv[++i];
        else if (key == "--dyn_preset" && i + 1 < argc)
            args.dyn_preset = argv[++i];
        else if (key == "--cc" && i + 1 < argc)
            args.controller_cc = std::stoi(argv[++i]);
        else if (key == "--out" && i + 1 < argc)
            args.output_file = argv[++i];
        else
        {
            std::cerr << "Unknown or malformed argument: " << key << "\n";
            throw std::runtime_error("Invalid arguments");
        }
    }

    return args;
}

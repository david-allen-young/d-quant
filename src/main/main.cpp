#include "CliParser.h"
#include "SingleNoteGenerator.h"
#include "JsonConfigLoader.h"
#include <iostream>
#include "../core/PathRegistry.h"

int main(int argc, char* argv[])
{
    try
    {
        MidiArgs args;
#if defined(_DEBUG)
        if (argc == 1)
        {
            // No command-line arguments passed
            std::cout << "[INFO] No arguments provided - loading default config file\n";
            if (!load_args_from_json("config_cli.json", args))
            {
                throw std::runtime_error("Could not load default_test_config.json");
            }
        }
        else
        {
            args = parse_args(argc, argv);
        }
#else
        args = parse_args(argc, argv);
#endif
        PathRegistry::loadFromFile("config.json");
        generate_single_note_midi(args);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}



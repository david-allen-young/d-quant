#include "../core/PathRegistry.h"
#include "PhraseArgs.h"
#include "PhraseJsonParser.h"
#include "RenderOptions.h"
#include "SongContext.h"
#include "SongContextLoader.h"
#include "PhraseRenderer.h"
#include <filesystem>
#include <iostream>

int main(int argc, char* argv[])
{
    PhraseArgs phrase;
    SongContext song;
    RenderOptions options;

    try
    {
#if defined(_DEBUG)
        if (argc == 1)
        {
            std::cout << "[INFO] No arguments provided - loading default phrase_cli.json\n";

            if (!std::filesystem::exists("phrase_cli.json"))
            {
                throw std::runtime_error("Missing default config: phrase_cli.json");
            }

            load_phrase_from_json("phrase_cli.json", phrase);

            if (std::filesystem::exists("song_context.json"))
            {
                load_song_context_from_json("song_context.json", song);
            }
            else
            {
                std::cout << "[INFO] No song_context.json found - using default song context.\n";
            }

            options.output_id = "debug_phrase";
        }
        else
#endif
        {
            std::string input_json_path;
            std::string song_json_path;

            for (int i = 1; i < argc; ++i)
            {
                std::string key = argv[i];
                if (key == "--input_json" && i + 1 < argc)
                {
                    input_json_path = argv[++i];
                }
                else if (key == "--output_id" && i + 1 < argc)
                {
                    options.output_id = argv[++i];
                }
                else if (key == "--song_json" && i + 1 < argc)
                {
                    song_json_path = argv[++i];
                }
                else
                {
                    std::cerr << "Unknown argument: " << key << "\n";
                    return 1;
                }
            }

            if (input_json_path.empty())
            {
                throw std::runtime_error("Missing required --input_json argument");
            }

            load_phrase_from_json(input_json_path, phrase);

            if (!song_json_path.empty())
            {
                load_song_context_from_json(song_json_path, song);
            }
        }

        PathRegistry::loadFromFile("config.json");
        generate_phrase_midi(phrase, song, options);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

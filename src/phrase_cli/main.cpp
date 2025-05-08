#include "PhraseArgs.h"
#include "PhraseJsonParser.h"
#include "PhraseRenderer.h"
#include "RenderOptions.h"
#include "SongContext.h"
#include "SongContextLoader.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 3 || argc > 5)
    {
        std::cerr << "Usage:\n  dquant_phrase_cli --input_json phrase.json [--output_id id] [--song_json song.json]\n";
        return 1;
    }

    std::string input_json_path;
    std::string song_json_path;
    RenderOptions options;
    PhraseArgs phrase;
    SongContext song;

    // Simple arg parser
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

    try
    {
        if (input_json_path.empty())
        {
            throw std::runtime_error("Missing --input_json argument");
        }

        load_phrase_from_json(input_json_path, phrase);

        if (!song_json_path.empty())
        {
            load_song_context_from_json(song_json_path, song);
        }
        else
        {
            std::cout << "[INFO] No song JSON provided. Using default tempo and dynamic preset.\n";
        }

        std::cout << "[INFO] Loaded " << phrase.notes.size() << " notes.\n";
        std::cout << "[INFO] Start dynamic: " << phrase.dyn_start << " -> " << phrase.dyn_end << "\n";

        // TODO: generate_phrase_midi(phrase, song, options);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    generate_phrase_midi(phrase, song, options);

    return 0;
}

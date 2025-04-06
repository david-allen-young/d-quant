//#include "dynamizer.h"
//#include <iostream>

//int main()
//{
//    std::string midiDir = "C:/GitHub/d-quant/assets/midi";
//    std::string midiFile = midiDir + "/Dynamizer_Training_CRE_pp_to_ff_0001.midi";
//    std::string midiEnvelopesDir = "C:/GitHub/d-quant/assets/envelopes_csv";
//
//    // Step 1: Convert MIDI to envelope CSVs
//    run_midi_to_envelope_csv(midiFile, midiEnvelopesDir);
//
//    // Step 2: Generate new envelopes via morph2 and export them
//    std::string morphResultsDir = "C:/GitHub/d-quant/assets/morph_csv";
//    int count = 100;
//
//    run_morph2_generation_example(midiEnvelopesDir, morphResultsDir, count);
//
//    std::cout << "Morph2 generation complete. Output in: " << morphResultsDir << "\n";
//
//    // Step 3: Apply a generated envelope as MIDI CC
//    std::string breathResultsDir = "C:/GitHub/d-quant/assets/breath_csv";
//    run_breath_cc_generation_example(morphResultsDir, breathResultsDir);
//
//	// Step 4: Generate an example note using the generated envelopes
//    run_note_builder_example(morphResultsDir, midiDir);
//
//    return 0;
//}

#include "CliParser.h"
#include "SingleNoteGenerator.h"
#include "JsonConfigLoader.h"
#include <iostream>

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
            if (!load_args_from_json("default_test_config.json", args))
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
        generate_single_note_midi(args);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}


#include "dynamizer.h"
#include <iostream>

int main()
{
    std::string midiDir = "C:/GitHub/d-quant/assets/midi";
    std::string midiFile = midiDir + "/Dynamizer_Training_CRE_pp_to_ff_0001.midi";
    std::string midiEnvelopesDir = "C:/GitHub/d-quant/assets/envelopes_csv";

    // Step 1: Convert MIDI to envelope CSVs
    run_midi_to_envelope_csv(midiFile, midiEnvelopesDir);

    // Step 2: Generate new envelopes via morph2 and export them
    std::string morphResultsDir = "C:/GitHub/d-quant/assets/morph_csv";
    int count = 100;

    run_morph2_generation_example(midiEnvelopesDir, morphResultsDir, count);

    std::cout << "Morph2 generation complete. Output in: " << morphResultsDir << "\n";

    // Step 3: Apply a generated envelope as MIDI CC
    std::string breathResultsDir = "C:/GitHub/d-quant/assets/breath_csv";
    run_breath_cc_generation_example(morphResultsDir, breathResultsDir);

	// Step 4: Generate an example note using the generated envelopes
    run_note_builder_example(morphResultsDir, midiDir);

    return 0;
}

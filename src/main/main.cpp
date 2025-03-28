#include "dynamizer.h"
#include <iostream>

int main()
{
    std::string midiFile = "C:/GitHub/d-quant/assets/midi/Dynamizer_Training_CRE_pp_to_ff_0001.midi";
    std::string midiEnvelopesDir = "C:/GitHub/d-quant/assets/envelopes_csv";

    // Step 1: Convert MIDI to envelope CSVs
    run_midi_to_envelope_csv(midiFile, midiEnvelopesDir);

    // Step 2: Generate new envelopes via morph2 and export them
    std::string morphResultsDir = "C:/GitHub/d-quant/assets/morph_csv";
    int count = 20;

    run_morph2_generation_example(midiEnvelopesDir, morphResultsDir, count);

    std::cout << "Morph2 generation complete. Output in: " << morphResultsDir << "\n";
    return 0;
}

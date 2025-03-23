#include "dynamizer.h"
#include <iostream>

int main()
{
    std::string midiFile = "C:/GitHub/d-quant/assets/midi/Dynamizer_Training_CRE_pp_to_ff_0001.midi";
    std::string outputDir = "C:/GitHub/d-quant/assets/output_csv";

    run_midi_to_envelope_csv(midiFile, outputDir);

    return 0;
}

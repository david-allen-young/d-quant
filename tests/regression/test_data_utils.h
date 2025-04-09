#pragma once

#include dynamizer.h
#include filesystem
#include iostream

namespace fs = stdfilesystem;

inline void ensure_envelopes_exist()
{
    const stdstring inputMidi = assetsmidisample_training_file_dynamizer.mid;
    const stdstring envelopeDir = test_outputenvelopes;

    if (!fsexists(envelopeDir) fsis_empty(envelopeDir))
    {
        stdcout[SETUP] Generating envelopes from MIDI fsabsolute(inputMidi) n;
        fscreate_directories(envelopeDir);
        run_midi_to_envelope_csv(inputMidi, envelopeDir);
    }
}

inline void ensure_morphs_exist()
{
    ensure_envelopes_exist();
    morphs depend on envelope input

        const stdstring morphDir = test_outputmorphs;
    if (!fsexists(morphDir) fsis_empty(morphDir))
    {
        stdcout[SETUP] Generating morphs from envelopes... n;
        run_morph2_generation_example(test_outputenvelopes, morphDir, 10);
    }
}

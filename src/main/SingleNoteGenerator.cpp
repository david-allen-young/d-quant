#include "SingleNoteGenerator.h"
#include <iostream>

void generate_single_note_midi(const MidiArgs& args)
{
    std::cout << "Generating single note MIDI file...\n";
    std::cout << "Note: " << args.note_number << "\n";
    std::cout << "Position (beats): " << args.position_beats << "\n";
    std::cout << "Duration (beats): " << args.duration_beats << "\n";
    std::cout << "Dynamics: " << args.dyn_start << " ? " << args.dyn_end << "\n";
    std::cout << "Dynamic preset: " << args.dyn_preset << "\n";
    std::cout << "Controller CC: " << args.controller_cc << "\n";
    std::cout << "Output file: " << args.output_file << "\n";

    // TODO:
    // - Apply rhythmizer to position/duration
    // - Use dynamizer to generate CC envelope (based on dyn_start -> dyn_end)
    // - Use intonizer to create pitch wheel envelope
    // - Write MIDI events and save file
}

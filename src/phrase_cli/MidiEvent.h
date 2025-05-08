#pragma once
#include <vector>
#include <cstdint>

enum class MidiEventType {
    NoteOn,
    NoteOff,
    ControlChange,
    PitchBend,
    Meta
};

struct MidiEvent {
    double timeBeats;                    // when the event happens (in beats)
    MidiEventType type;                 // what kind of event
    std::vector<uint8_t> message;      // raw MIDI bytes to emit
};

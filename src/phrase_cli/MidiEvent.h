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
        MidiEvent() {}
        MidiEvent(double timeBeats, MidiEventType type, std::vector<uint8_t> message)
            : timeBeats(timeBeats), type(type), message(message)
        {
        }
    double timeBeats;                    // when the event happens (in beats)
    MidiEventType type;                 // what kind of event
    std::vector<uint8_t> message;      // raw MIDI bytes to emit
};

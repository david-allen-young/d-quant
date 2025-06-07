#include "MidiPhraseBuilder.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

// Helper to convert beat time to MIDI ticks
static uint32_t beatsToTicks(double beats, int tpqn)
{
    return static_cast<uint32_t>(std::max(0.0, std::round(beats * tpqn)));
}

void MidiPhraseBuilder::addNote(const NoteInterface& note)
{
    double startBeat = note.getPositionInBeats();
    double endBeat = startBeat + note.getDurationInBeats();

    // === Note On
    {
        std::vector<uint8_t> noteOnMessage = {0x90,
                                              static_cast<uint8_t>(note.getKeyNumber()),
                                              static_cast<uint8_t>(note.getVelocity())};
        MidiEvent noteOnEvent(startBeat, MidiEventType::NoteOn, noteOnMessage);
        events.push_back(noteOnEvent);
    }

    // === Note Off
    {
        std::vector<uint8_t> noteOffMessage = {0x80,
                                               static_cast<uint8_t>(note.getKeyNumber()),
                                               static_cast<uint8_t>(0)};
        MidiEvent noteOffEvent(endBeat, MidiEventType::NoteOff, noteOffMessage);
        events.push_back(noteOffEvent);
    }
}


void MidiPhraseBuilder::addExpression(const std::vector<std::pair<double, int>>& envelope)
{
    for (const auto& [beat, ccVal] : envelope)
    {
        std::vector<uint8_t> message = {0xB0,
            static_cast<uint8_t>(static_cast<int>(cc)),
            static_cast<uint8_t>(ccVal)
        };
        MidiEvent midiEvent(beat, MidiEventType::ControlChange, message);
        events.push_back(midiEvent);
    }
}

void MidiPhraseBuilder::addIntonation(const std::vector<Point>& envelope)
{
    for (const auto& [beat, /*pitchNorm*/pbVal] : envelope)
    {
        //int pbVal = static_cast<int>(pitchNorm * 8192.0);
        int unsigned14 = pbVal + 8192;
        uint8_t lsb = static_cast<uint8_t>(unsigned14 & 0x7F);
        uint8_t msb = static_cast<uint8_t>((unsigned14 >> 7) & 0x7F);
        std::vector<uint8_t> message = {0xE0, lsb, msb};
        MidiEvent midiEvent(beat, MidiEventType::PitchBend, message);
        events.push_back(midiEvent);
    }
}

void MidiPhraseBuilder::addNote(const NoteInterface& note, int tpqn, MidiController cc)
{
    double startBeat = note.getPositionInBeats();
    double endBeat = startBeat + note.getDurationInBeats();

    // === Note On
    events.push_back({startBeat,
                      MidiEventType::NoteOn,
                      {0x90, static_cast<uint8_t>(note.getKeyNumber()), static_cast<uint8_t>(note.getVelocity())}});

    // === Note Off
    events.push_back({endBeat,
                      MidiEventType::NoteOff,
                      {0x80, static_cast<uint8_t>(note.getKeyNumber()), 0x00}});

    // === Expression (CC)
    for (const auto& [beat, valueNorm] : note.getExpression())
    {
        int ccVal = std::clamp(static_cast<int>(valueNorm * 127.0), 0, 127);
        events.push_back({beat,
                          MidiEventType::ControlChange,
                          {0xB0, static_cast<uint8_t>(static_cast<int>(cc)), static_cast<uint8_t>(ccVal)}});
    }

    // === Pitch Bend
    for (const auto& [beat, pitchNorm] : note.getIntonation())
    {
        int pitchVal = static_cast<int>(pitchNorm * 8192.0);
        int unsigned14 = pitchVal + 8192;
        uint8_t lsb = static_cast<uint8_t>(unsigned14 & 0x7F);
        uint8_t msb = static_cast<uint8_t>((unsigned14 >> 7) & 0x7F);

        events.push_back({beat,
                          MidiEventType::PitchBend,
                          {0xE0, lsb, msb}});
    }
}

void MidiPhraseBuilder::writeToFile(const std::string& outPath)
{
    writeToFile(outPath, tpqn);
}

void MidiPhraseBuilder::writeToFile(const std::string& outPath, int tpqn)
{
    // Sort events by time
    std::sort(events.begin(), events.end(), [](const MidiEvent& a, const MidiEvent& b)
              { return a.timeBeats < b.timeBeats; });

    std::ofstream out(outPath, std::ios::binary);
    if (!out.is_open())
    {
        throw std::runtime_error("Could not open MIDI file for writing: " + outPath);
    }

    // === Write MIDI Header
    out.write("MThd", 4);
    MidiFileWriter::writeUint32BE(out, 6); // header length
    MidiFileWriter::writeUint16BE(out, 1); // format type 1
    MidiFileWriter::writeUint16BE(out, 1); // one track
    MidiFileWriter::writeUint16BE(out, tpqn);

    // === Collect track data
    std::vector<uint8_t> trackData;
    uint32_t lastTick = 0;

    for (const auto& event : events)
    {
        uint32_t tick = beatsToTicks(event.timeBeats, tpqn);
        uint32_t delta = (tick >= lastTick) ? tick - lastTick : 0;

        MidiFileWriter::writeVariableLengthQuantity(trackData, delta);
        trackData.insert(trackData.end(), event.message.begin(), event.message.end());

        lastTick = tick;
    }

    // === End of Track
    trackData.push_back(0x00);
    trackData.push_back(0xFF);
    trackData.push_back(0x2F);
    trackData.push_back(0x00);

    // === Write Track Chunk
    out.write("MTrk", 4);
    MidiFileWriter::writeUint32BE(out, static_cast<uint32_t>(trackData.size()));
    out.write(reinterpret_cast<char*>(trackData.data()), trackData.size());
}

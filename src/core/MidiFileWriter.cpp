#include "MidiFileWriter.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>

void MidiFileWriter::writeSingleNoteFile(const NoteInterface& note,
                                         const std::string& outPath,
                                         int ticksPerQuarterNote)
{
    std::vector<uint8_t> trackData;
    uint32_t lastTick = 0;

    // Convert beats to ticks
    auto beatsToTicks = [ticksPerQuarterNote](double beats) -> uint32_t
    {
        return static_cast<uint32_t>(std::max(0.0, std::round(beats * ticksPerQuarterNote)));
    };

    // === [1] Meta Marker at tick 0 ===
    const double noteOnBeat = note.getPositionInBeats() + 1.0;
    {
        uint32_t markerTick = beatsToTicks(noteOnBeat - 1.0);
        uint32_t delta = (markerTick >= lastTick)
                             ? (markerTick - lastTick)
                             : 0; // clamp negative to 0 — safest fallback
        writeVariableLengthQuantity(trackData, delta);

        trackData.push_back(0xFF);                 // Meta event
        trackData.push_back(0x06);                 // Marker type
        std::string label = "DQUANT_PREROLL";
        trackData.push_back(static_cast<uint8_t>(label.size()));
        trackData.insert(trackData.end(), label.begin(), label.end());
        lastTick = markerTick;
    }

    // === [2] Expression Preroll Handling ===
    // We'll shift expression points earlier so that the *last* preroll CC lands at beat 1.0
    const auto& expression = note.getExpression();
    size_t prerollCCCount = std::min<size_t>(2, expression.size());

    double prerollBeats = 0.0;
    if (expression.size() >= 2)
    {
        prerollBeats = expression[std::min<size_t>(prerollCCCount - 1, expression.size() - 1)].first - expression[0].first + 0.01; // small buffer
    }

    const double noteStart = noteOnBeat;

    // === [3] Shifted Expression (CC2) - PRE-NOTE
    for (const auto& [beat, ccValueNorm] : expression)
    {
        double shiftedBeat = std::max(0.0, beat + (noteStart - prerollBeats));
        uint32_t tick = beatsToTicks(shiftedBeat);

        if (tick >= beatsToTicks(noteStart))
            continue; // skip post-note CC2s for now

        writeVariableLengthQuantity(trackData, tick - lastTick);
        trackData.push_back(0xB0); // Control Change, channel 0
        trackData.push_back(2);    // CC2: Breath Controller
        trackData.push_back(static_cast<uint8_t>(ccValueNorm * 127.0));
        lastTick = tick;
    }

    // === [4] Note On
    {
        uint32_t tick = beatsToTicks(noteStart);
        writeVariableLengthQuantity(trackData, tick - lastTick);
        trackData.push_back(0x90); // Note On, channel 0
        trackData.push_back(static_cast<uint8_t>(note.getKeyNumber()));
        trackData.push_back(static_cast<uint8_t>(note.getVelocity()));
        lastTick = tick;
    }

    // === [5] Shifted Expression (CC2) - POST-NOTE
    for (const auto& [beat, ccValueNorm] : expression)
    {
        double shiftedBeat = std::max(0.0, beat + (noteStart - prerollBeats));
        uint32_t tick = beatsToTicks(shiftedBeat);

        if (tick < beatsToTicks(noteStart))
            continue; // already written

        writeVariableLengthQuantity(trackData, tick - lastTick);
        trackData.push_back(0xB0); // Control Change, channel 0
        trackData.push_back(2);    // CC2: Breath Controller
        trackData.push_back(static_cast<uint8_t>(ccValueNorm * 127.0));
        lastTick = tick;
    }

    double noteEnd = noteStart + note.getDurationInBeats();

    // === [6] Note Off
    {
        uint32_t tick = lastTick;
        writeVariableLengthQuantity(trackData, tick - lastTick);
        trackData.push_back(0x80); // Note Off
        trackData.push_back(static_cast<uint8_t>(note.getKeyNumber()));
        trackData.push_back(0); // velocity 0
        lastTick = tick;
    }

    // End of Track Meta Event
    {
        writeVariableLengthQuantity(trackData, 0);
        trackData.push_back(0xFF);
        trackData.push_back(0x2F);
        trackData.push_back(0x00);
    }

    std::cout << "Writing note to " << outPath
              << "\n  NoteOn @ beat " << noteStart
              << "\n  Duration: " << note.getDurationInBeats() << " beats"
              << "\n  Preroll: " << prerollBeats << " beats\n";


    // Build the full MIDI file
    std::ofstream outFile(outPath, std::ios::binary);

    // Header chunk "MThd"
    outFile.write("MThd", 4);
    writeUint32BE(outFile, 6); // header length
    writeUint16BE(outFile, 1); // format type 1
    writeUint16BE(outFile, 1); // one track
    writeUint16BE(outFile, ticksPerQuarterNote);

    // Track chunk "MTrk"
    outFile.write("MTrk", 4);
    writeUint32BE(outFile, static_cast<uint32_t>(trackData.size()));
    outFile.write(reinterpret_cast<char*>(trackData.data()), trackData.size());
}

void MidiFileWriter::writeVariableLengthQuantity(std::vector<uint8_t>& buffer, uint32_t value)
{
    uint8_t bytes[5]; // Defensive: MIDI VLQ never uses more than 4 bytes, but room for safety
    int count = 0;
    do
    {
        if (count >= 5)
        {
            throw std::runtime_error("VLQ encoding error: value too large");
        }
        bytes[count++] = value & 0x7F;
        value >>= 7;
    } while (value > 0);

    for (int i = count - 1; i >= 0; --i)
    {
        uint8_t byte = bytes[i];
        if (i != 0)
            byte |= 0x80;
        buffer.push_back(byte);
    }
}

void MidiFileWriter::writeUint32BE(std::ofstream& out, uint32_t value)
{
    uint8_t bytes[4] = {
        static_cast<uint8_t>((value >> 24) & 0xFF),
        static_cast<uint8_t>((value >> 16) & 0xFF),
        static_cast<uint8_t>((value >> 8) & 0xFF),
        static_cast<uint8_t>(value & 0xFF)};
    out.write(reinterpret_cast<char*>(bytes), 4);
}

void MidiFileWriter::writeUint16BE(std::ofstream& out, uint16_t value)
{
    uint8_t bytes[2] = {
        static_cast<uint8_t>((value >> 8) & 0xFF),
        static_cast<uint8_t>(value & 0xFF)};
    out.write(reinterpret_cast<char*>(bytes), 2);
}

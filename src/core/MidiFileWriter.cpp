#include "MidiFileWriter.h"
#include <algorithm>
#include <cstdint>
#include <fstream>

void MidiFileWriter::writeSingleNoteFile(const NoteInterface& note,
                                         const std::string& outPath,
                                         int ticksPerQuarterNote)
{
    std::vector<uint8_t> trackData;
    uint32_t lastTick = 0;

    // Convert beats to ticks
    auto beatsToTicks = [ticksPerQuarterNote](double beats) -> uint32_t
    {
        return static_cast<uint32_t>(beats * ticksPerQuarterNote);
    };

    // Note On
    {
        uint32_t tick = beatsToTicks(note.getPositionInBeats());
        writeVariableLengthQuantity(trackData, tick - lastTick);
        trackData.push_back(0x90); // Note On, channel 0
        trackData.push_back(static_cast<uint8_t>(note.getKeyNumber()));
        trackData.push_back(static_cast<uint8_t>(note.getVelocity()));
        lastTick = tick;
    }

    // Expression (Breath Controller - CC #2)
    for (const auto& [beat, ccValueNorm] : note.getExpression())
    {
        uint32_t tick = beatsToTicks(beat);
        writeVariableLengthQuantity(trackData, tick - lastTick);
        trackData.push_back(0xB0); // CC message, channel 0
        trackData.push_back(2);    // CC #2 Breath Controller
        trackData.push_back(static_cast<uint8_t>(ccValueNorm * 127.0));
        lastTick = tick;
    }

    // Note Off
    {
        uint32_t tick = beatsToTicks(note.getPositionInBeats() + note.getDurationInBeats());
        writeVariableLengthQuantity(trackData, tick - lastTick);
        trackData.push_back(0x80); // Note Off, channel 0
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
    uint8_t bytes[4];
    int count = 0;
    do
    {
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

#pragma once
#include "Note.h"
#include <vector>
#include <fstream>
#include <string>

class MidiFileWriter
{
public:
	static void writeSingleNoteFile(const NoteInterface& note, const std::string& outPath, int ticksPerQuarterNote);
    static void writeVariableLengthQuantity(std::vector<uint8_t>& buffer, uint32_t value);
	static void writeUint32BE(std::ofstream& out, uint32_t value);
	static void writeUint16BE(std::ofstream& out, uint16_t value);
};

#include "MidiFileReader_v2.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

static const uint32_t MIDI_HEADER_LENGTH = 6;
static const int MIDI_CHUNK_SIZE = 4;
static const std::string MIDI_HEADER_CHUNK_ID = "MThd";
static const std::string MIDI_TRACK_CHUNK_ID = "MTrk";

uint32_t bitSwap32(uint32_t value)
{
    return (value >> 24) | ((value & 0x00FF0000) >> 8) | ((value & 0x0000FF00) << 8) | (value << 24);
}

uint16_t bitSwap16(uint16_t value)
{
    return (value >> 8) | (value << 8);
}

uint32_t read_uint32(std::ifstream& file)
{
    uint32_t valueBigEndian;
    file.read(reinterpret_cast<char*>(&valueBigEndian), sizeof(valueBigEndian));
    uint32_t valueLittleEndian = bitSwap32(valueBigEndian);
    return valueLittleEndian;
}

uint16_t read_uint16(std::ifstream& file)
{
	uint16_t valueBigEndian;
	file.read(reinterpret_cast<char*>(&valueBigEndian), sizeof(valueBigEndian));
	uint16_t valueLittleEndian = bitSwap16(valueBigEndian);
	return valueLittleEndian;
}

void MidiFileReader_v2::readMidi(std::string filename)
{
    tracks.clear();
    readMidiType1(filename);
}

void MidiFileReader_v2::readMidiType1(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }
    uint16_t track_count;
    char header[MIDI_CHUNK_SIZE];
    readMidiType1Header(file, header, track_count);
    for (int track_idx = 0; track_idx < track_count; ++track_idx)
    {
        auto track_data = readMidiType1Track(file, header, track_idx);
        tracks.push_back(track_data);
    }
    file.close();
}

void MidiFileReader_v2::readMidiType1Header(std::ifstream& file, char* header, uint16_t& track_count)
{
    // Read the header chunk ("MThd")
    file.read(header, MIDI_CHUNK_SIZE);
    if (file.gcount() != MIDI_CHUNK_SIZE)
    {
        std::cerr << "Error: Failed to read MThd!" << std::endl;
        return;
    }
    if (std::string(header, MIDI_CHUNK_SIZE) != MIDI_HEADER_CHUNK_ID)
    {
        std::cerr << "Invalid MIDI header." << std::endl;
        return;
    }

    // Read the header length (always 6 for a standard MIDI file)
    uint32_t header_length = read_uint32(file);
    std::cout << "Header length: " << header_length << std::endl;
    if (header_length != MIDI_HEADER_LENGTH)
    {
        std::cerr << "Invalid header length." << std::endl;
        return;
    }

    // Read the format type (should be 1 for Type 1)
    uint16_t format_type = read_uint16(file);
    std::cout << "Format type: " << format_type << std::endl;
    if (format_type != 1)
    {
        std::cerr << "This is not a Type 1 MIDI file." << std::endl;
        return;
    }

    // Read the number of tracks
    track_count = read_uint16(file);
    std::cout << "Track count: " << track_count << std::endl;

    // Read division (2 bytes)
    uint16_t division = read_uint16(file);
    std::cout << "Division: " << division << std::endl;

    std::cout << "After reading header, file position: " << file.tellg() << std::endl;
}

std::vector<char> MidiFileReader_v2::readMidiType1Track(std::ifstream& file, char* header, int track_idx)
{
    std::cout << "Reading Track " << (track_idx + 1) << " at file position: " << file.tellg() << std::endl;

    // Read the track chunk ("MTrk")
    file.read(header, MIDI_CHUNK_SIZE);
    if (std::string(header, MIDI_CHUNK_SIZE) != MIDI_TRACK_CHUNK_ID)
    {
        std::cerr << "Invalid track header." << std::endl;
        return {};
    }

    // Read the track length
    uint32_t track_length = read_uint32(file);
    std::cout << "Track " << (track_idx + 1) << " length: " << track_length << " bytes" << std::endl;

    // Read the track data
    std::vector<char> track_data(track_length);
    file.read(track_data.data(), track_length);
    return track_data;
}



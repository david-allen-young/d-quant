#pragma once
#include <string>
#include <vector>

class MidiFileReader_v2
{
public:
    void readMidi(std::string filename);
    std::vector<std::vector<char>> getTracks() const { return tracks; }

private:
    std::vector<std::vector<char>> tracks;
    void readMidiType1(const std::string& filename);
    void readMidiType1Header(std::ifstream& file, char* header, uint16_t& track_count);
    std::vector<char> readMidiType1Track(std::ifstream& file, char* header, int track_idx);
};

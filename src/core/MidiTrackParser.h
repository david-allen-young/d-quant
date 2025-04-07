#pragma once
#include <iostream>
#include <vector>
class Point;
class Note;

class MidiTrackParser
{
public:
    MidiTrackParser() = default;
    void parseTrackEvents(std::vector<char>& track_data, std::vector<Point>& envelope);
    void extractNoteEvents(std::vector<char>& trackData, std::vector<Note>& notes);

private:
    uint32_t readVariableLengthQuantity(const std::vector<char>& track_data, int& index);
};

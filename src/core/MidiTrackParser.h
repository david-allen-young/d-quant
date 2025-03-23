#pragma once
#include <iostream>
#include <vector>
class Point;

class MidiTrackParser
{
public:
    MidiTrackParser() = default;
    void parseTrackEvents(std::vector<char>& track_data, std::vector<Point>& envelope);

private:
    uint32_t readVariableLengthQuantity(const std::vector<char>& track_data, int& index);
};

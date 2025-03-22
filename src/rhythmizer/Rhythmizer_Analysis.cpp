// Rhythmizer_Analysis.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "AnalysisHelpers.h"
#include "MidiFileReader_v2.h"
#include "MidiTrackParser.h"
#include <iostream>
#include <cassert>

void printMidiTrack(std::vector<char>& track_data)
{
    // Here we would parse the track events
    // (not fully implemented in this example)
    // For now, just print the raw track data
    for (size_t i = 0; i < track_data.size(); ++i)
    {
        std::cout << std::hex << static_cast<int>(static_cast<unsigned char>(track_data[i])) << " ";
        if ((i + 1) % 16 == 0)
        {
            std::cout << std::endl;
        }
    }
    std::cout << std::dec << std::endl;
}

int main()
{
	// test writeCSV
	if(false)
	{
		std::vector<Point> envelope;
		envelope.push_back({ 0.0, 0.0 });
		envelope.push_back({ 0.5, 0.10 });
		envelope.push_back({ 1.0, 0.15 });
		envelope.push_back({ 1.5, 0.20 });
		envelope.push_back({ 2.0, 0.25 });
		envelope.push_back({ 2.5, 0.35 });
		envelope.push_back({ 3.0, 0.47 });
		envelope.push_back({ 3.5, 0.63 });
		envelope.push_back({ 4.0, 0.89 });
		std::string dir = "C:\\GitHub\\rhythmizer\\Rhythmizer_Visualization\\";
		std::string file = dir + "envelope.csv";
		writeCSV(envelope, file);
	}

	// test MidiFileReader
    if (true)
    {
        std::string path = "C:/GitHub/rhythmizer/assets/";
        std::string filename = path + "Dynamizer_Training_CRE_pp_to_ff_0001.midi";
        MidiFileReader_v2 midiFile;
        midiFile.readMidi(filename);
        auto tracks = midiFile.getTracks();
        for (int trackIdx = 1; trackIdx < tracks.size(); ++trackIdx)
        {
            std::cout << "Track " << trackIdx << " data:\n";
            //printMidiTrack(tracks[trackIdx]);
            MidiTrackParser parser;
            std::vector<Point> breathEvents;
            parser.parseTrackEvents(tracks[trackIdx], breathEvents);
            std::string dir = "C:\\GitHub\\rhythmizer\\Rhythmizer_Visualization\\";
            std::string file = dir + "envelope" + std::to_string(trackIdx) + ".csv";
            writeCSV(breathEvents, file);
			std::cout << std::endl;
        }
    }

	return 0;
}


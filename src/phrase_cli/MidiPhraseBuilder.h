#pragma once
#include "MidiEvent.h"
#include "Note.h"
#include "MidiFileWriter.h"
#include <string>
#include <vector>

class MidiPhraseBuilder {
public:
    void addNote(const NoteInterface& note, int tpqn, MidiController cc);
    void writeToFile(const std::string& outPath, int tpqn);

private:
    std::vector<MidiEvent> events;
};

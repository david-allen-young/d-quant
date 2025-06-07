#pragma once
#include "Point.h"
#include "MidiEvent.h"
#include "Note.h"
#include "MidiFileWriter.h"
#include <string>
#include <vector>

class MidiPhraseBuilder {
public:
        MidiPhraseBuilder()
        : tpqn(480), cc(MidiController::Breath)
        {
        }
        MidiPhraseBuilder(int tpqn, MidiController cc)
            : tpqn(tpqn), cc(cc)
        {
        }
    void addNote(const NoteInterface& note, int tpqn, MidiController cc);
    void addNote(const NoteInterface& note);
    void addExpression(const std::vector<std::pair<double, int>>& envelope);
    void addIntonation(const std::vector<Point>& envelope);
    void writeToFile(const std::string& outPath, int tpqn);
    void writeToFile(const std::string& outPath);

private:
    const int tpqn;
    const MidiController cc;
    std::vector<MidiEvent> events;
};

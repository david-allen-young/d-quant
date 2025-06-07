#include "PhraseJsonParser.h"
#include <fstream>
#include <stdexcept>

#include <json.hpp>
using json = nlohmann::json;

bool load_phrase_from_json(const std::string& path, PhraseArgs& out_phrase)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open phrase JSON file: " + path);
    }

    json j;
    file >> j;

    // Phrase-level
    const auto& phrase = j.at("phrase");
    out_phrase.slur = phrase.value("slur", false);
    out_phrase.dyn_start = phrase.value("dyn_start", "mf");
    out_phrase.dyn_end = phrase.value("dyn_end", "mf");

    // Notes
    const auto& notes = j.at("notes");
    for (const auto& note_json : notes)
    {
        NoteArgs note;
        note.pitch = note_json.at("pitch");
        note.duration_beats = note_json.value("duration", 1.0f);
        note.articulation = note_json.value("articulation", "ordinario");
        note.accent = note_json.value("accent", "none");
        out_phrase.notes.push_back(note);
    }

    return true;
}

#include "PhraseRenderer.h"
#include <cmath>
#include <iostream>
#include <map>

#include "CsvWriters.h"
#include "rhythmizer.h"
#include "RandomCsvFileSelector.h"
#include "../core/PathRegistry.h"
#include "AnalysisHelpers.h" // for readCSV
#include <filesystem>

#include "dynamizer.h" // or "ExpressionGenerator.h"
using dynamizer::ExpressionMark;
using dynamizer::markFromStr;

#include "NoteBuilder.h"
#include "IntonationGenerator.h"
#include "intonizer.h"
#include "ExpressionGenerator.h"
#include "EnvelopeUtils.h"
#include "MidiPhraseBuilder.h"

#include "ExpressionMarks.h"

namespace
{

// MIDI note conversion (e.g., "C4" → 60)
int pitchToMidi(const std::string& name)
{
    static const std::map<std::string, int> baseNotes = {
        {"C", 0}, {"C#", 1}, {"D", 2}, {"D#", 3}, {"E", 4}, {"F", 5}, {"F#", 6}, {"G", 7}, {"G#", 8}, {"A", 9}, {"A#", 10}, {"B", 11}};

    if (name.size() < 2)
        return 60; // default to C4
    std::string letter = name.substr(0, name.length() - 1);
    int octave = name.back() - '0';
    auto it = baseNotes.find(letter);
    if (it == baseNotes.end())
        return 60;

    return 12 * (octave + 1) + it->second;
}

} // anonymous namespace

void generate_phrase_midi(const PhraseArgs& phrase, const SongContext& context, const RenderOptions& options)
{
    std::cout << "[DEBUG] Output ID: " << options.output_id << "\n";
    std::cout << "[DEBUG] Dynamic Preset: " << context.dyn_preset << "\n";
    std::cout << "[DEBUG] Slur applied: " << (phrase.slur ? "Yes" : "No") << "\n";

    //float current_time = 0.0f;

    //for (size_t i = 0; i < phrase.notes.size(); ++i)
    //{
    //    const auto& note = phrase.notes[i];
    //    int midi = pitchToMidi(note.pitch);

    //    std::cout << "Note " << i + 1 << ": pitch=" << note.pitch
    //              << " (MIDI " << midi << "), duration=" << note.duration_beats
    //              << ", starts at beat " << current_time
    //              << ", articulation=" << note.articulation
    //              << ", accent=" << note.accent << "\n";

    //    current_time += note.duration_beats;
    //}

    std::vector<std::tuple<double, double, double>> rhythmDeltas;
    std::vector<Point> envelopePoints;

    float current_time = 0.0f;

    auto morphsBase = PathRegistry::getResolvedPath("dynamizer_generation").lexically_normal();

    ExpressionMark dynStart = markFromStr(phrase.dyn_start);
    ExpressionMark dynEnd = markFromStr(phrase.dyn_end);

    std::string subdir = (dynStart < dynEnd)   ? "crescendo"
                         : (dynStart > dynEnd) ? "diminuendo"
                                               : "stable";

    std::filesystem::path morphDir = morphsBase / subdir;

    auto envelopeCsv = selectRandomCsvInDir(morphDir.string());
    std::cout << "[DEBUG] Envelope CSV: " << envelopeCsv << "\n";
    readCSV(envelopeCsv, envelopePoints); // (Position, Expression)

    // === [TIMING LOOP] ===
    for (size_t i = 0; i < phrase.notes.size(); ++i)
    {
        const auto& note = phrase.notes[i];
        int midiNoteNum = pitchToMidi(note.pitch);

        double nominalPos = current_time;
        double nominalDur = note.duration_beats;

        NoteBuilderMidi builder;
        builder.setKeyNumber(midiNoteNum);

        // Rhythmizer modifies duration + position in place
        rhythmizer::applyTiming(builder, nominalPos, nominalDur, note.articulation);

        double actualPos = builder.getData().positionInBeats;
        double actualDur = builder.getData().durationInBeats;

        double delta = actualPos - nominalPos;
        double ratio = actualDur / nominalDur;

        rhythmDeltas.emplace_back(nominalPos, delta, ratio);

        std::cout << "Note " << i + 1 << ": pitch=" << note.pitch
                  << " (MIDI " << midiNoteNum << "), nominal=" << nominalPos
                  << ", actual=" << actualPos
                  << ", dur=" << nominalDur << "->" << actualDur
                  << ", articulation=" << note.articulation
                  << ", accent=" << note.accent << "\n";

        current_time += nominalDur;
    }

    // === Rescale breath and pitch envelope across full phrase ===
    double phraseDuration = current_time; // already accumulated
    std::vector<Point> envelope;

    for (const auto& pt : envelopePoints)
    {
        envelope.emplace_back(pt.time * phraseDuration, pt.value);
    }

    //// Generate pitch bend from breathCC using intonizer
    //auto dynStart = markFromStr(phrase.dyn_start);
    //auto dynEnd = markFromStr(phrase.dyn_end);
    //double centsPerDeltaCC = 1.2;
    //double compensation = (dynStart == dynEnd) ? 1.25 : 0.25;

    ////auto dynPreset = getRangeForPreset(presetFromStr(args.dyn_preset));
    //auto dynPreset = dynamizer::getRangeForPreset(dynamizer::presetFromStr("pp-to-ff"));
    //auto breathCC = generateBreathCCFromEnvelope(envelope, phraseDuration, dynStart, dynEnd, dynPreset.first, dynPreset.second);
    //

    //auto pitchEnv = intonizer::applyPitchEnvelope(breathCC, dynStart == dynEnd, centsPerDeltaCC, compensation);
    

    MidiPhraseBuilder phraseBuilder;
    int tpqn = 480;
    double currentBeat = 0.0;

    //auto dynPreset = getRangeForPreset(presetFromStr(args.dyn_preset));
    auto dynPreset = dynamizer::getRangeForPreset(dynamizer::presetFromStr(context.dyn_preset));

    double centsPerDeltaCC = 1.2;
    double compensation = (dynStart == dynEnd) ? 1.25 : 0.25;

    auto fullPhraseBreathCC = generateBreathCCFromEnvelope(envelope, phraseDuration, dynStart, dynEnd, dynPreset.first, dynPreset.second);
    auto fullPhrasePitchBend = intonizer::applyPitchEnvelope(fullPhraseBreathCC, dynStart == dynEnd, centsPerDeltaCC, compensation);

    for (size_t i = 0; i < phrase.notes.size(); ++i)
    {
        const auto& note = phrase.notes[i];
        int midiNoteNum = pitchToMidi(note.pitch);

        double nominalDur = note.duration_beats;
        double start = currentBeat;
        double dur = nominalDur;

        NoteBuilderMidi builder;
        builder.setKeyNumber(midiNoteNum);

        rhythmizer::applyTiming(builder, start, dur, note.articulation);
        builder.setVelocity(64); // Will override below

        double end = start + dur;
        //auto rawSegment = extractEnvelopeSegment(envelope, start, end);

        // === Partition & load expression
        //auto ccSegment = generateBreathCCFromEnvelope(rawSegment, dur, dynStart, dynEnd, dynPreset.first, dynPreset.second);
        auto ccSegment = extractEnvelopeSegment(fullPhraseBreathCC, start, end);
        for (const auto& [t, val] : ccSegment)
        {
            builder.addExpression(t, val);
        }

        // Velocity
        size_t velocityIdx = std::min(2ULL, std::max(0ULL, ccSegment.size() - 1));
        if (ccSegment.empty())
        {
            throw std::runtime_error("Expression envelope empty");
        }
        builder.setVelocity(ccSegment[velocityIdx].second);

        // === Partition & load pitch bend
        //auto pbSegment = intonizer::applyPitchEnvelope(ccSegment, dynStart == dynEnd, centsPerDeltaCC, compensation);
        auto pbSegment = extractEnvelopeSegment(fullPhrasePitchBend, start, end);
        for (const auto& pt : pbSegment)
        {
            builder.addIntonation(pt.time, pt.value);
        }

        auto notePtr = builder.build();
        phraseBuilder.addNote(*notePtr, tpqn, MidiController::Breath);

        currentBeat += nominalDur;
    }


    //std::cout << "[INFO] (Stub) Would write MIDI + envelope CSV to: "
    //          << options.output_dir << "/" << options.output_id << ".mid/.csv\n";

    const auto csvOutDir = (PathRegistry::getResolvedPath("working_dir_cli") / "csv").lexically_normal();
    std::filesystem::create_directories(csvOutDir);

    auto envPath = (csvOutDir / (options.output_id + "_dynamics.csv")).string();
    auto rhyPath = (csvOutDir / (options.output_id + "_rhythm.csv")).string();

    std::vector<std::pair<double, double>> ccAsPairs;
    for (const auto& pt : envelope)
    {
        double val = pt.value;
        if (val < 1.0 / 256.0)
        {
            val = 0;
        }
        ccAsPairs.emplace_back(pt.time, val);
    }

    writeEnvelopeCsv(envPath, ccAsPairs);
    writeRhythmizerCsv(rhyPath, rhythmDeltas);

    std::cout << "[INFO] Wrote envelope to: " << envPath << "\n";
    std::cout << "[INFO] Wrote rhythm data to: " << rhyPath << "\n";

    auto midiOutDir = (PathRegistry::getResolvedPath("working_dir_cli") / "midi").lexically_normal();
    std::filesystem::create_directories(midiOutDir);
    auto midiOutPath = (midiOutDir / (options.output_id + ".mid")).string();

    phraseBuilder.writeToFile(midiOutPath, tpqn);
    std::cout << "[INFO] Wrote MIDI to: " << midiOutPath << "\n";
}

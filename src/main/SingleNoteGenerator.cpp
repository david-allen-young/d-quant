// #include "SingleNoteGenerator.h"
// #include <iostream>
//
// void generate_single_note_midi(const MidiArgs& args)
//{
//     std::cout << "Generating single note MIDI file...\n";
//     std::cout << "Note: " << args.note_number << "\n";
//     std::cout << "Position (beats): " << args.position_beats << "\n";
//     std::cout << "Duration (beats): " << args.duration_beats << "\n";
//     std::cout << "Dynamics: " << args.dyn_start << " - " << args.dyn_end << "\n";
//     std::cout << "Dynamic preset: " << args.dyn_preset << "\n";
//     std::cout << "Controller CC: " << args.controller_cc << "\n";
//     std::cout << "Output file: " << args.output_file << "\n";
//
//     // TODO:
//     // - Apply rhythmizer to position/duration
//     // - Use dynamizer to generate CC envelope (based on dyn_start -> dyn_end)
//     // - Use intonizer to create pitch wheel envelope
//     // - Write MIDI events and save file
// }

#include "SingleNoteGenerator.h"
#include "ExpressionGenerator.h" // for generateBreathCCFromEnvelope
#include "rhythmizer.h"
//#include "intonizer.h"
#include "MidiArgs.h"
#include "MidiFileWriter.h"
#include "NoteBuilder.h"
#include "AnalysisHelpers.h"
#include <filesystem>
#include <random>
#include <iostream>

using dynamizer::ExpressionMark;
using dynamizer::markFromStr;
using dynamizer::getRangeForPreset;
using dynamizer::presetFromStr;
using dynamizer::generateBreathCCFromEnvelope;

//using rhythmizer::applyTiming;

void generate_single_note_midi(const MidiArgs& args)
{
    // === [1] Create the base NoteBuilder ===
    NoteBuilderMidi builder;
    builder.setKeyNumber(args.note_number);
    //builder.setVelocity(64); // neutral, or map from args later
    //builder.setPosition(args.position_beats);
    //builder.setDuration(args.duration_beats);

    // === [2] Rhythmizer stub (future): just passthrough for now ===
    // Eventually: rhythmizer::process(builder); -> tweak position/duration
    double nominalPosition = args.position_beats;
    double nominalDuration = args.duration_beats;
    rhythmizer::applyTiming(builder, nominalPosition, nominalDuration, args.articulation_preset);

    // === [3] Dynamizer: Generate envelope -> CC ===
    std::vector<std::string> envelopePaths;
    std::vector<std::vector<Point>> morphedEnvelopes;
    //const std::string morphResultsDir = "C:/GitHub/d-quant/assets/morph_csv";
    const std::string& morphResultsDir = args.morph_csv_dir;
    for (const auto& entry : std::filesystem::directory_iterator(morphResultsDir))
    {
        if (entry.path().extension() == ".csv")
        {
            std::vector<Point> points;
            readCSV(entry.path().string(), points);
            if (!points.empty())
            {
                morphedEnvelopes.push_back(points);
                envelopePaths.push_back(entry.path().string());
            }
        }
    }
    if (morphedEnvelopes.empty())
    {
        throw std::runtime_error("No morph envelopes found in: " + morphResultsDir);
    }
    ExpressionMark dynStart = markFromStr(args.dyn_start);
    ExpressionMark dynEnd = markFromStr(args.dyn_end);
    //ExpressionMark dynMin = ExpressionMark::pp;
    //ExpressionMark dynMax = ExpressionMark::ff;
    auto dynPreset = getRangeForPreset(presetFromStr(args.dyn_preset));

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> indexDist(0, (int)morphedEnvelopes.size() - 1);
    int rIdx = indexDist(rng);

    auto envelope = morphedEnvelopes[rIdx];
    // After selecting random envelope:
    std::cout << "Using envelope: " << envelopePaths[rIdx] << std::endl;

    auto breathCC = generateBreathCCFromEnvelope(envelope,
                                                 /*args.duration_beats,*/
                                                 builder.getData().durationInBeats,
                                                 dynStart,
                                                 dynEnd,
                                                 /*dynMin*/dynPreset.first,
                                                 /*dynMax*/dynPreset.second);

    for (const auto& [pos, cc] : breathCC)
        builder.addExpression(pos, cc);

    size_t velocityIdx = std::min(2ULL, std::max(0ULL, breathCC.size() - 1));
    if (breathCC.empty())
    {
        throw std::runtime_error("Expression envelope empty");
    }
    builder.setVelocity(breathCC[velocityIdx].second);

    // === [4] Intonizer stub ===
    // intonizer::applyIntonation(builder);
    // builder.addIntonation(...); <- Not yet implemented

    // === [5] Finalize note and write to MIDI ===
    auto note = builder.build();
    MidiFileWriter writer;
    int tpqn = 480;
    writer.writeSingleNoteFile(*note, args.output_file, tpqn, args.controller_cc);
}

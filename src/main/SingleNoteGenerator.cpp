#include "SingleNoteGenerator.h"
#include "ExpressionGenerator.h" // for generateBreathCCFromEnvelope
#include "rhythmizer.h"
#include "intonizer.h"
#include "IntonationGenerator.h"
#include "RandomCsvFileSelector.h"
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

void generate_single_note_midi(const MidiArgs& args)
{
    // === [1] Create the base NoteBuilder ===
    NoteBuilderMidi builder;
    builder.setKeyNumber(args.note_number);

    // === [2] Rhythmizer : Apply timing adjustments ===
    double nominalPosition = args.position_beats;
    double nominalDuration = args.duration_beats;
    rhythmizer::applyTiming(builder, nominalPosition, nominalDuration, args.articulation_preset);

    // === [3] Dynamizer: Generate envelope -> CC ===
    //std::vector<std::string> envelopePaths;
    //std::vector<std::vector<Point>> morphedEnvelopes;

    ExpressionMark dynStart = markFromStr(args.dyn_start);
    ExpressionMark dynEnd = markFromStr(args.dyn_end);

    std::cout << "dynStart: " << static_cast<int>(dynStart)
              << " | dynEnd: " << static_cast<int>(dynEnd) << std::endl;

    std::string subdir;
    if (dynStart < dynEnd)
        subdir = "crescendo";
    else if (dynStart > dynEnd)
        subdir = "diminuendo";
    else
        subdir = "stable";


    std::filesystem::path morphDir = (std::filesystem::path(args.morph_csv_dir) / subdir / "generated").lexically_normal();
    std::cout << "Selected morph directory: " << morphDir << std::endl;

    auto envelopePath = selectRandomCsvInDir(morphDir.string());
    std::cout << "Using envelope: " << envelopePath << std::endl;
    std::vector<Point> points;
    readCSV(envelopePath, points);
    auto envelope = points;

    //for (const auto& entry : std::filesystem::directory_iterator(morphDir))
    //{
    //    if (entry.path().extension() == ".csv")
    //    {
    //        std::vector<Point> points;
    //        readCSV(entry.path().string(), points);
    //        if (!points.empty())
    //        {
    //            morphedEnvelopes.push_back(points);
    //            envelopePaths.push_back(entry.path().string());
    //        }
    //    }
    //}
    //if (morphedEnvelopes.empty())
    //{
    //    throw std::runtime_error("No morph envelopes found in: " + morphDir.string());
    //}

    //std::cout << "[DEBUG] Found " << envelopePaths.size() << " morph files:\n";
    //for (const auto& p : envelopePaths)
    //    std::cout << "  " << p << std::endl;


    auto dynPreset = getRangeForPreset(presetFromStr(args.dyn_preset));

    //std::mt19937 rng(std::random_device{}());
    //std::uniform_int_distribution<> indexDist(0, (int)morphedEnvelopes.size() - 1);
    //int rIdx = indexDist(rng);

    //auto envelope = morphedEnvelopes[rIdx];
    //// After selecting random envelope:
    //std::cout << "Using envelope: " << envelopePaths[rIdx] << std::endl;

    auto breathCC = generateBreathCCFromEnvelope(envelope,
                                                 builder.getData().durationInBeats,
                                                 dynStart,
                                                 dynEnd,
                                                 dynPreset.first,
                                                 dynPreset.second);

    std::cout << "[DEBUG] Expression CC mapping:" << std::endl;
    for (const auto& [pos, cc] : breathCC)
    {
        std::cout << "  beat " << pos << " -> CC " << cc << std::endl;
    }

    for (const auto& [pos, cc] : breathCC)
        builder.addExpression(pos, cc);

    size_t velocityIdx = std::min(2ULL, std::max(0ULL, breathCC.size() - 1));
    if (breathCC.empty())
    {
        throw std::runtime_error("Expression envelope empty");
    }
    builder.setVelocity(breathCC[velocityIdx].second);

    // === [4] Intonizer stub ===
    double centsPerDeltaCC = 1.2;
    double compensation = (dynStart != dynEnd) ? 0.98 : 1.0;
    //double compensation = 1.0;
    auto pitchEnvelope = intonizer::applyPitchEnvelope(breathCC, dynStart == dynEnd, centsPerDeltaCC, compensation);

    std::cout << "[DEBUG] Pitch Bend mapping:\n";
    for (const auto& [pos, pitch] : pitchEnvelope)
    {
        std::cout << "  beat " << pos << " -> PB " << static_cast<int>(pitch) << std::endl;
    }

    for (auto& pt : pitchEnvelope)
    {
        builder.addIntonation(pt.time, pt.value);
    }

    // === [5] Finalize note and write to MIDI ===
    auto note = builder.build();
    MidiFileWriter writer;
    int tpqn = 480;
    writer.writeSingleNoteFile(*note, args.output_file, tpqn, args.controller_cc);
}

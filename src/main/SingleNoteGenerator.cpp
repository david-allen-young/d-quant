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

#include "../core/PathRegistry.h"


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
    ExpressionMark dynStart = markFromStr(args.dyn_start);
    ExpressionMark dynEnd = markFromStr(args.dyn_end);
    std::cout << "dynStart: " << static_cast<int>(dynStart) << " | dynEnd: " << static_cast<int>(dynEnd) << std::endl;
    std::string subdir;
    if (dynStart < dynEnd)
    {
        subdir = "crescendo";
    }
    else if (dynStart > dynEnd)
    {
        subdir = "diminuendo";
    }
    else
    {
        subdir = "stable";
    }
    auto morphsBase = PathRegistry::getResolvedPath("dynamizer_generation").lexically_normal();
    std::filesystem::path morphDir =  (morphsBase / subdir).lexically_normal();
    std::cout << "Selected morph directory: " << morphDir << std::endl;
    auto envelopePath = selectRandomCsvInDir(morphDir.string());
    std::cout << "Using envelope: " << envelopePath << std::endl;
    std::vector<Point> points;
    readCSV(envelopePath, points);
    auto envelope = points;
    auto dynPreset = getRangeForPreset(presetFromStr(args.dyn_preset));
    auto breathCC = generateBreathCCFromEnvelope(envelope, builder.getData().durationInBeats, dynStart, dynEnd, dynPreset.first, dynPreset.second);
    std::cout << "[DEBUG] Expression CC mapping:" << std::endl;
    for (const auto& [pos, cc] : breathCC)
    {
        std::cout << "  beat " << pos << " -> CC " << cc << std::endl;
    }
    for (const auto& [pos, cc] : breathCC)
    {
        builder.addExpression(pos, cc);
    }
    size_t velocityIdx = std::min(2ULL, std::max(0ULL, breathCC.size() - 1));
    if (breathCC.empty())
    {
        throw std::runtime_error("Expression envelope empty");
    }
    builder.setVelocity(breathCC[velocityIdx].second);

    // === [4] Intonizer stub ===
    double centsPerDeltaCC = 1.2;
    double compensation = (dynStart == dynEnd) ? 1.25 : 0.25;
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
    const auto cliBase = PathRegistry::getResolvedPath("working_dir_cli");
    const auto outputDir = (cliBase / "midi").lexically_normal();
    const auto outputPath = (outputDir / args.output_file).lexically_normal();
    writer.writeSingleNoteFile(*note, outputPath.string(), tpqn, args.controller_cc);
}

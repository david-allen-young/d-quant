#include "AnalysisHelpers.h"
#include "ExpressionGenerator.h"
#include "MidiFileWriter.h"
#include "NoteBuilder.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <iostream>
#include <random>

#include "PipelineArgsRegistry.h"
using namespace dynamizer;
namespace fs = std::filesystem;

TEST_CASE("NoteBuilder can write an expressive note to MIDI", "[regression]")
{
    const auto& args = getPipelineArgs();
    const std::string inputDir = args.morph_csv_dir;
    const std::string outputDir = args.output_dir + "/midi";
    fs::create_directories(outputDir);

    // Load a random envelope
    std::vector<std::vector<Point>> envelopes;
    for (const auto& entry : fs::directory_iterator(inputDir))
    {
        if (entry.path().extension() == ".csv")
        {
            std::vector<Point> points;
            readCSV(entry.path().string(), points);
            if (!points.empty())
                envelopes.push_back(points);
        }
    }

    REQUIRE(!envelopes.empty());
    std::mt19937 rng(std::random_device{}());
    auto envelope = envelopes[std::uniform_int_distribution<>(0, envelopes.size() - 1)(rng)];

    // === Build expressive note ===
    double durationInBeats = 4.0;
    ExpressionMark start = ExpressionMark::pp;
    ExpressionMark end = ExpressionMark::ff;

    auto ccData = generateBreathCCFromEnvelope(envelope, durationInBeats, start, end, start, end);

    NoteBuilderMidi builder;
    builder.setKeyNumber(60);              // Middle C
    builder.setVelocity(ccData[0].second); // Use early CC value
    builder.setPosition(0.0);
    builder.setDuration(durationInBeats);

    for (const auto& [pos, cc] : ccData)
        builder.addExpression(pos, cc);

    auto note = builder.build();
    CHECK(note != nullptr);

    // === Save to MIDI file ===
    std::string outPath = outputDir + "/expressive_note.mid";
    MidiFileWriter writer;
    writer.writeSingleNoteFile(*note, outPath, 480, MidiController::Breath);
    std::cout << "Wrote: " << fs::absolute(outPath) << std::endl;

    CHECK(fs::exists(outPath));
}

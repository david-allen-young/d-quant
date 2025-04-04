#include "dynamizer.h"
#include "AnalysisHelpers.h"


void run_envelope_csv_example()
{
    std::vector<Point> envelope =
    {
        {0.0, 0.0},
        {0.5, 0.10},
        {1.0, 0.15},
        {1.5, 0.20},
        {2.0, 0.25},
        {2.5, 0.35},
        {3.0, 0.47},
        {3.5, 0.63},
        {4.0, 0.89}
    };

    std::string file = "assets/output_csv/envelope.csv";
    writeCSV(envelope, file);
}

#include "MidiFileReader_v2.h"
#include "MidiTrackParser.h"
#include <iostream>
#include <vector>

void run_midi_to_envelope_csv(const std::string& midiFilePath, const std::string& outputDir)
{
    MidiFileReader_v2 midiFile;
    midiFile.readMidi(midiFilePath);
    std::cout << "Reading " << midiFilePath << std::endl;

    auto tracks = midiFile.getTracks();
    for (int trackIdx = 1; trackIdx < tracks.size(); ++trackIdx)
    {
        std::cout << "Track " << trackIdx << " data:\n";

        MidiTrackParser parser;
        std::vector<Point> breathEvents;
        parser.parseTrackEvents(tracks[trackIdx], breathEvents);

        std::string file = outputDir + "/envelope" + std::to_string(trackIdx) + ".csv";
        writeCSV(breathEvents, file);
        std::cout << "Wrote " << file << std::endl;
    }
}

#include "EnvelopeUtils.h"
#include <filesystem>
#include <random>

namespace fs = std::filesystem;

void run_morph2_generation_example(const std::string& inputDir,
                                   const std::string& outputDir,
                                   int count)
{
    const size_t targetLength = 100;

    // 1. Load and interpolate envelopes
    std::vector<std::vector<double>> interpolatedEnvelopes;

    for (const auto& entry : fs::directory_iterator(inputDir))
    {
        if (entry.path().extension() == ".csv")
        {
            std::vector<Point> points;
            readCSV(entry.path().string(), points);
            if (!points.empty())
            {
                auto interp = interpolateEnvelope(points, targetLength);
                interpolatedEnvelopes.push_back(std::move(interp));
            }
        }
    }

    if (interpolatedEnvelopes.size() < 2)
    {
        std::cerr << "Need at least 2 envelopes to perform morph2 generation.\n";
        return;
    }

    // 2. Compute mean and std
    std::vector<double> mean, std;
    analyzeEnvelopes(interpolatedEnvelopes, mean, std);

    // 3. Random generator
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<> morphDist(0.2, 0.8);
    std::uniform_int_distribution<> indexDist(0, (int)interpolatedEnvelopes.size() - 1);

    // 4. Generate morphs
    fs::create_directories(outputDir);
    for (int i = 0; i < count; ++i)
    {
        std::vector<double> env1, env2;

        // Find a pair with high directional agreement
        while (true)
        {
            int idx1 = indexDist(rng);
            int idx2 = indexDist(rng);
            if (idx1 == idx2)
            {
                continue;
            }

            const auto& e1 = interpolatedEnvelopes[idx1];
            const auto& e2 = interpolatedEnvelopes[idx2];

            // Directional agreement
            int agree = 0;
            for (size_t j = 0; j < targetLength; ++j)
            {
                int d1 = (e1[j] - mean[j]) > 0 ? 1 : -1;
                int d2 = (e2[j] - mean[j]) > 0 ? 1 : -1;
                if (d1 == d2)
                {
                    ++agree;
                }
            }

            if ((double)agree / targetLength > 0.8)
            {
                env1 = e1;
                env2 = e2;
                break;
            }
        }

        Morph2Params params{
            /*.mean =*/ mean,
            /*.input1 =*/ env1,
            /*.input2 =*/ env2,
            /*.std =*/ std,
            /*.morphFactor1 =*/ morphDist(rng),
            /*.morphFactor2 =*/ morphDist(rng),
            /*.softness =*/ 3.0};

        auto generated = morphFromTwoInputs(params);

        // Write to CSV for Python visualizer
        std::vector<Point> csvPoints(targetLength);
        for (size_t j = 0; j < targetLength; ++j)
        {
            csvPoints[j].time = static_cast<double>(j) / (targetLength - 1); // normalized 0–1
            csvPoints[j].value = generated[j];
        }

        std::string outPath = outputDir + "/generated_morph2_" + std::to_string(i + 1) + ".csv";
        writeCSV(csvPoints, outPath);
    }

    std::cout << "Generated " << count << " envelopes using morph2.\n";
}

#include "ExpressionGenerator.h"

void run_breath_cc_generation_example(const std::string& inputDir, const std::string& outputDir)
{
    //// Example envelope (normalized 0–1 time and value)
    //std::vector<Point> envelope = {
    //    {0.0, 0.0},
    //    {0.2, 0.1},
    //    {0.4, 0.3},
    //    {0.6, 0.6},
    //    {0.8, 0.9},
    //    {1.0, 1.0}};

    std::vector<std::vector<Point>> morphedEnvelopes;
    for (const auto& entry : fs::directory_iterator(inputDir))
    {
        if (entry.path().extension() == ".csv")
        {
            std::vector<Point> points;
            readCSV(entry.path().string(), points);
            if (!points.empty())
            {
                morphedEnvelopes.push_back(points);
            }
        }
    }

    double durationBeats = 4.0;
    ExpressionMark start = ExpressionMark::pp;
    ExpressionMark end = ExpressionMark::ff;
    ExpressionMark minScore = ExpressionMark::pp;
    ExpressionMark maxScore = ExpressionMark::ff;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> indexDist(0, (int)morphedEnvelopes.size() - 1);
    int rIdx = indexDist(rng);

    auto envelope = morphedEnvelopes[rIdx];
    auto breathCC = generateBreathCCFromEnvelope(envelope, durationBeats, start, end, minScore, maxScore);

    std::vector<Point> output;
    for (const auto& [beat, ccVal] : breathCC)
    {
        output.push_back({beat, static_cast<double>(ccVal)});
    }

    //std::string file = "assets/output_csv/breath_cc_example.csv";
    std::string file = outputDir + "/breath_cc_example.csv";
    writeCSV(output, file);

    std::cout << "Wrote test breath CC output to: " << file << std::endl;
}

#include "NoteBuilder.h"
#include "MidiFileWriter.h"
//#include <random>

void run_note_builder_example(const std::string& envelopeDir, const std::string& outputDir)
{
    std::vector<std::vector<Point>> envelopes;
    for (const auto& entry : std::filesystem::directory_iterator(envelopeDir))
    {
        if (entry.path().extension() == ".csv")
        {
            std::vector<Point> points;
            readCSV(entry.path().string(), points);
            if (!points.empty())
                envelopes.push_back(points);
        }
    }

    if (envelopes.empty())
    {
        std::cerr << "No envelopes found in " << envelopeDir << std::endl;
        return;
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> indexDist(0, static_cast<int>(envelopes.size() - 1));
    auto envelope = envelopes[indexDist(rng)];

    // Params
    double durationInBeats = 4.0;
    ExpressionMark start = ExpressionMark::pp;
    ExpressionMark end = ExpressionMark::ff;
    ExpressionMark minScore = ExpressionMark::pp;
    ExpressionMark maxScore = ExpressionMark::ff;

    // Convert envelope to breath CC values
    auto breathCC = generateBreathCCFromEnvelope(envelope, durationInBeats, start, end, minScore, maxScore);

    // Build note with expression
    NoteBuilderMidi builder;
    builder.setKeyNumber(60); // Middle C
    builder.setVelocity(64);  // Neutral MIDI velocity
    builder.setPosition(0.0); // Start at beat 0
    builder.setDuration(durationInBeats);
    //builder.setExpressionMarkRange(minScore, maxScore);

    for (const auto& [pos, cc] : breathCC)
        builder.addExpression(pos, cc);

    auto note = builder.build();

    // Inspect or hold note for MIDI output
    std::cout << "Note built: pitch " << note->getKeyNumber()
              << ", duration " << note->getDurationInBeats()
              << ", with " << note->getExpression().size()
              << " expression points." << std::endl;

    // You can store this note or pass it to a future midi writer
    MidiFileWriter writer;
    int ticksPerQuarterNote = 480;
    std::string filename = outputDir + "/expressive_note.mid";
    writer.writeSingleNoteFile(*note, filename, ticksPerQuarterNote);
    std::cout << "Saved generated note to file: " << filename << std::endl;
}


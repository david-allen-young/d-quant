#include "dynamizer.h"
#include "AnalysisHelpers.h"
#include "MidiFileReader_v2.h"
#include "MidiTrackParser.h"

#include <iostream>
#include <vector>

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
            readCSV(entry.path().string(), points); // your existing CSV reader
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

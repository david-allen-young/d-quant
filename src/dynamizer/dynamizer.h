#pragma once
#include <string>

void run_envelope_csv_example();
void run_midi_to_envelope_csv(const std::string& midiFilePath, const std::string& outputDir);
void run_morph2_generation_example(const std::string& inputDir, const std::string& outputDir, int count);
void run_breath_cc_generation_example(const std::string& inputDir, const std::string& outputDir);
void run_note_builder_example(const std::string& envelopeDir, const std::string& outputDir);
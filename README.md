# d-quant

**d-quant** is a C++17 toolkit for expressive MIDI generation, modeling performance nuance through data-driven analysis and synthesis of rhythm and dynamics. It provides command-line tools and modular libraries for analyzing performance envelopes, rhythm deviations, and generating expressive single-note MIDI events.

---

## Features

- 🎼 **Expression Modeling**: Analyze MIDI CC (e.g., breath) to extract and morph dynamic envelopes.
- 🥁 **Rhythmic Nuance**: Learn and apply microtiming and articulation-based rhythm deviations.
- 🧪 **Envelope Synthesis**: Use statistical methods (mean/std/morph2) to synthesize expressive control curves.
- 🎹 **Single Note Generator**: CLI tool to create expressive single-note MIDI files with user-defined dynamics and articulation.
- 🧰 **Modular Design**: Cleanly separated modules for rhythm, dynamics, pitch (future), and core MIDI tools.

---

## Project Structure

```plaintext
src/
├── core/            # Envelope utilities, MIDI parsing, note builders
├── rhythmizer/      # Rhythm variation generator
├── dynamizer/       # Expression envelope analysis and synthesis
├── intonizer/       # (Stub) Pitch/intensity shaping
├── main/            # CLI entrypoint and single-note MIDI generation
tests/
└── unit/            # Catch2-based unit tests
tools/
├── analyze_rhythm/  # Rhythm analysis tools
└── analyze_dynamics/ # Dynamics/envelope analysis
```

---

## Build Instructions

```bash
git clone https://github.com/yourusername/d-quant.git
cd d-quant
mkdir build && cd build
cmake ..
make
```

> ❗️ In-source builds are disallowed — please build in a separate directory.

---

## CLI Example

Generate a single expressive MIDI note with custom articulation and dynamics:

```bash
./dquant_cli   --note 60   --pos 1.0   --dur 4.0   --art_preset "tenuto"   --dyn_start "pp"   --dyn_end "ff"   --dyn_preset "pp_to_ff"   --cc 2   --out "note_output.mid"
```

Or use a config file:

```bash
./dquant_cli --config path/to/config.json
```

---

## Output CSV Examples

- `envelope.csv`: Normalized CC envelopes over time
- `rhythm_deviation.csv`: Deviation from nominal timing, duration, and velocity
- `breath_cc_example.csv`: Converted envelope mapped to CC values

These can be visualized using Python scripts in the companion repo `d-quant-visualizer`.

---

## Upcoming

- 🎯 **Pitch (Intonation) Modeling**
- 📊 GitHub Actions for automatic MIDI → CSV analysis
- 🎵 Batch expressive MIDI file generation

---

## License

Apache 2 License. See `LICENSE` file.

---

## Acknowledgments

Developed as part of the d-quant project for expressive performance synthesis and analysis.

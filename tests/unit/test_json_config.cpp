#include "main/JsonConfigLoader.h"
#include "main/MidiArgs.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <fstream>

TEST_CASE("load_args_from_json correctly parses config file", "[json_config]")
{
    const std::string filename = "test_config.json";

    // Create a minimal config
    std::ofstream file(filename);
    REQUIRE(file.is_open());
    file << R"({
        "note_number": 72,
        "position_beats": 2.0,
        "duration_beats": 1.25,
        "dyn_start": "mp",
        "dyn_end": "ff",
        "controller_cc": 11,
        "output_file": "example.mid"
    })";
    file.close();

    MidiArgs args;
    bool success = load_args_from_json(filename, args);

    REQUIRE(success);
    CHECK(args.note_number == 72);
    CHECK(args.position_beats == Catch::Approx(2.0));
    CHECK(args.duration_beats == Catch::Approx(1.25));
    CHECK(args.dyn_start == "mp");
    CHECK(args.dyn_end == "ff");
    CHECK(args.controller_cc == MidiController(11));
    CHECK(args.output_file == "example.mid");

    std::remove(filename.c_str()); // Clean up
}

#include "main/CliParser.h"
#include "main/MidiArgs.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("parse_args correctly parses basic CLI arguments", "[cli]")
{
    const char* argv[] = {
        "dquant", // argv[0] is usually program name
        "--note", "60",
        "--dur", "1.5",
        "--dyn_start", "pp",
        "--dyn_end", "ff",
        "--cc", "2",
        "--out", "test.mid"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    MidiArgs args = parse_args(argc, const_cast<char**>(argv));

    CHECK(args.note_number == 60);
    CHECK(args.duration_beats == Catch::Approx(1.5));
    CHECK(args.dyn_start == "pp");
    CHECK(args.dyn_end == "ff");
    CHECK(args.controller_cc == MidiController(2));
    CHECK(args.output_file == "test.mid");
}

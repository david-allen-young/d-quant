#include "AnalysisHelpers.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("writeCSV and readCSV round-trip preserves envelope data", "[csv]")
{
    std::vector<Point> original = {
        {0.0, 0.0},
        {0.5, 0.25},
        {1.0, 0.5},
        {1.5, 0.75},
        {2.0, 1.0}};

    const std::string filename = "test_roundtrip.csv";

    writeCSV(original, filename);

    std::vector<Point> loaded;
    bool success = readCSV(filename, loaded);

    REQUIRE(success);
    REQUIRE(loaded.size() == original.size());

    for (size_t i = 0; i < original.size(); ++i)
    {
        CHECK(loaded[i].time == Catch::Approx(original[i].time));
        CHECK(loaded[i].value == Catch::Approx(original[i].value));
    }

    std::remove(filename.c_str()); // Clean up temp file
}

#include "NoteBuilder.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("NoteBuilder constructs note with correct properties", "[note_builder]")
{
    NoteBuilderMidi builder;

    int key = 64;
    int velocity = 96;
    double position = 1.0;
    double duration = 2.5;

    builder.setKeyNumber(key);
    builder.setVelocity(velocity);
    builder.setPosition(position);
    builder.setDuration(duration);

    // Add a few expression points
    builder.addExpression(0.0, 32);
    builder.addExpression(1.0, 64);
    builder.addExpression(2.0, 96);

    auto note = builder.build();

    REQUIRE(note != nullptr);
    const NoteData& data = note->getNoteData();

    CHECK(data.keyNumber == key);
    CHECK(data.velocity == velocity);
    CHECK(data.positionInBeats == Catch::Approx(position));
    CHECK(data.durationInBeats == Catch::Approx(duration));
    CHECK(data.expression.size() == 3);
    CHECK(data.expression[1].second == Catch::Approx(64.0 / 127.0));
}

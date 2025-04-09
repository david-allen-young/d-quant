#include "NoteBuilder.h"
#include "rhythmizer.h"
#include <catch2/catch_test_macros.hpp>

using namespace rhythmizer;

TEST_CASE("applyTiming trims duration according to articulation style", "[rhythmizer]")
{
    const double nominalPosition = 0.0;
    const double nominalDuration = 1.0;

    NoteBuilderMidi builderStaccato;
    NoteBuilderMidi builderRegular;
    NoteBuilderMidi builderTenuto;

    applyTiming(builderStaccato, nominalPosition, nominalDuration, "staccato");
    applyTiming(builderRegular, nominalPosition, nominalDuration, "normal");
    applyTiming(builderTenuto, nominalPosition, nominalDuration, "tenuto");

    const double durStaccato = builderStaccato.getData().durationInBeats;
    const double durRegular = builderRegular.getData().durationInBeats;
    const double durTenuto = builderTenuto.getData().durationInBeats;

    INFO("Staccato: " << durStaccato);
    INFO("Regular:  " << durRegular);
    INFO("Tenuto:   " << durTenuto);

    CHECK(durStaccato < durRegular);
    CHECK(durRegular < durTenuto);
    CHECK(durTenuto < nominalDuration);
}

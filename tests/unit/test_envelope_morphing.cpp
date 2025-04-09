#include "EnvelopeUtils.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("morphFromTwoInputs generates values within expected range", "[morph]")
{
    const size_t N = 5;
    Morph2Params params{
        /*.mean = */{0.2, 0.4, 0.6, 0.8, 1.0},
        /*.input1 = */{0.3, 0.5, 0.7, 0.9, 1.0},
        /*.input2 = */{0.1, 0.3, 0.5, 0.7, 1.0},
        /*.std = */{0.1, 0.1, 0.1, 0.1, 0.1},
        /*.morphFactor1 = */0.5,
        /*.morphFactor2 = */0.5,
        /*.softness = */3.0};

    std::vector<double> result = morphFromTwoInputs(params);

    REQUIRE(result.size() == N);
    for (size_t i = 0; i < N; ++i)
    {
        CHECK(result[i] >= 0.0);
        CHECK(result[i] <= 1.0);
    }
}

#pragma once
#include <utility>

// Defines standard Western dynamics for use in expression modeling
enum class ExpressionMark
{
    niente = 0,
    pppp = 1,
    ppp = 2,
    pp = 3,
    p = 4,
    mp = 5,
    mf = 6,
    f = 7,
    ff = 8,
    fff = 9,
    ffff = 10
};

enum class DynamicRangePreset
{
    pp_to_ff,
    ppp_to_fff,
    pppp_to_ffff
};

inline std::pair<ExpressionMark, ExpressionMark> getRangeForPreset(DynamicRangePreset preset)
{
    switch (preset)
    {
    case DynamicRangePreset::pp_to_ff:
        return {ExpressionMark::pp, ExpressionMark::ff};
    case DynamicRangePreset::ppp_to_fff:
        return {ExpressionMark::ppp, ExpressionMark::fff};
    case DynamicRangePreset::pppp_to_ffff:
        return {ExpressionMark::pppp, ExpressionMark::ffff};
    default:
        return {ExpressionMark::niente, ExpressionMark::ffff};
    }
}

// Convert a symbolic expression mark to a MIDI controller index (0–127),
// assuming the expression marks span a min to max range.
inline int expressionMarkToCC(ExpressionMark mark, ExpressionMark min, ExpressionMark max)
{
    int minVal = static_cast<int>(min);
    int maxVal = static_cast<int>(max);
    int val = static_cast<int>(mark);

    if (maxVal == minVal)
        return 64; // Avoid divide-by-zero

    double norm = static_cast<double>(val - minVal) / (maxVal - minVal);
    return static_cast<int>(norm * 127.0);
}

#include "ExpressionMarks.h"

namespace dynamizer
{
//-------------------------------------------------------------------------------------------

pair<ExpressionMark, ExpressionMark> getRangeForPreset(DynamicRangePreset preset)
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
int expressionMarkToCC(ExpressionMark mark, ExpressionMark min, ExpressionMark max)
{
    int minVal = static_cast<int>(min);
    int maxVal = static_cast<int>(max);
    int val = static_cast<int>(mark);

    if (maxVal == minVal)
        return 64; // Avoid divide-by-zero

    double norm = static_cast<double>(val - minVal) / (maxVal - minVal);
    return static_cast<int>(norm * 127.0);
}

// Map dynamic string (e.g., "pp") to ExpressionMark
ExpressionMark markFromStr(const string& mark)
{
    if (mark == "pppp")
        return ExpressionMark::pppp;
    if (mark == "ppp")
        return ExpressionMark::ppp;
    if (mark == "pp")
        return ExpressionMark::pp;
    if (mark == "p")
        return ExpressionMark::p;
    if (mark == "mp")
        return ExpressionMark::mp;
    if (mark == "mf")
        return ExpressionMark::mf;
    if (mark == "f")
        return ExpressionMark::f;
    if (mark == "ff")
        return ExpressionMark::ff;
    if (mark == "fff")
        return ExpressionMark::fff;
    if (mark == "ffff")
        return ExpressionMark::ffff;
    return ExpressionMark::niente;
};

DynamicRangePreset presetFromStr(const string& preset)
{
    if (preset == "ppp_to_fff")
    {
        return DynamicRangePreset::ppp_to_fff;
    }
    if (preset == "pppp_to_ffff")
    {
        return DynamicRangePreset::pppp_to_ffff;
    }
    return DynamicRangePreset::pp_to_ff;
}

//-------------------------------------------------------------------------------------------
} // namespace dynamizer
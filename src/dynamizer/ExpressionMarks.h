#pragma once
#include <utility>
#include <string>

namespace dynamizer
{
//-------------------------------------------------------------------------------------------

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

std::pair<ExpressionMark, ExpressionMark> getRangeForPreset(DynamicRangePreset preset);
int expressionMarkToCC(ExpressionMark mark, ExpressionMark min, ExpressionMark max);
ExpressionMark markFromStr(const std::string& mark);
DynamicRangePreset presetFromStr(const std::string& preset);

//-------------------------------------------------------------------------------------------
} // namespace dynamizer
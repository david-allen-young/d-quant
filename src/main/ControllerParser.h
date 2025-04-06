#pragma once
#include "MidiArgs.h"
#include <algorithm>
#include <stdexcept>

inline MidiController parseController(const std::string& value)
{
    if (value == "modulation")
        return MidiController::Modulation;
    if (value == "breath")
        return MidiController::Breath;
    if (value == "expression")
        return MidiController::Expression;

    if (std::all_of(value.begin(), value.end(), ::isdigit))
    {
        int cc = std::stoi(value);
        switch (cc)
        {
        case 1:
            return MidiController::Modulation;
        case 2:
            return MidiController::Breath;
        case 11:
            return MidiController::Expression;
        default:
            throw std::runtime_error("Invalid controller number: must be 1, 2, or 11");
        }
    }

    throw std::runtime_error("Invalid controller: must be one of [1, 2, 11] or [modulation, breath, expression]");
}
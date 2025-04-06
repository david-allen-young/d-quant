#pragma once
#include "MidiArgs.h"
#include <string>

bool load_args_from_json(const std::string& path, MidiArgs& out_args);

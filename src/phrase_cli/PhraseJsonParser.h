#pragma once
#include "PhraseArgs.h"
#include <string>

bool load_phrase_from_json(const std::string& path, PhraseArgs& out_phrase);

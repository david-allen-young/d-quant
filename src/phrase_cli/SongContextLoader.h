#pragma once
#include "SongContext.h"
#include <string>

bool load_song_context_from_json(const std::string& path, SongContext& out_context);

#pragma once
#include <string>

struct RenderOptions {
    std::string output_id = "output";        // stem name for .mid/.csv/.png
    std::string output_dir = "../output";    // relative or absolute output base
};

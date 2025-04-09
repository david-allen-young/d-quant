#pragma once
#include "PipelineArgs.h"
#include <string>

bool load_args_from_json(const std::string& path, PipelineArgs& out_args);

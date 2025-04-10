#pragma once
#include "PipelineArgs.h"

// Called from main() during test startup
void initPipelineArgs(int argc, char* argv[]);

// Used from within tests
const PipelineArgs& getPipelineArgs();

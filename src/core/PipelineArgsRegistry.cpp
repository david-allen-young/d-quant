#include "PipelineArgsRegistry.h"
#include "PipelineConfigLoader.h"

static PipelineArgs g_args;

void initPipelineArgs(int argc, char* argv[])
{
    g_args = getEffectivePipelineArgs(argc, argv);
}

const PipelineArgs& getPipelineArgs()
{
    return g_args;
}

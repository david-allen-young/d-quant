#define CATCH_CONFIG_RUNNER
#include "PipelineArgsRegistry.h"
#include <catch2/catch_session.hpp>

int main(int argc, char* argv[])
{
    initPipelineArgs(argc, argv);
    return Catch::Session().run(argc, argv);
}

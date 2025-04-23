#define CATCH_CONFIG_RUNNER
#include "PipelineArgsRegistry.h"
#include "core/PathRegistry.h"
#include <catch2/catch_session.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        // Load path config before test logic runs
        PathRegistry::loadFromFile("config.json");
        PathRegistry::loadFromEnv();

        // Load pipeline-specific CLI args (existing behavior)
        initPipelineArgs(argc, argv);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "[ERROR] Registry initialization failed: " << ex.what() << std::endl;
        return 1;
    }

    // Start Catch2
    return Catch::Session().run(argc, argv);
}

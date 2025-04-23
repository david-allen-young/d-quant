#define CATCH_CONFIG_RUNNER
#include "PipelineArgsRegistry.h"
#include "core/PathRegistry.h"
#include <catch2/catch_session.hpp>
#include <iostream>

void cleanUpGeneratedTestFiles()
{
    using std::filesystem::remove_all;
    try
    {
        auto testOutDir = PathRegistry::getResolvedPath("working_dir_tests");
        remove_all(testOutDir);
        std::cout << "[INFO] Cleaned up test output directory: " << testOutDir << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "[WARN] Cleanup failed: " << ex.what() << std::endl;
    }
}


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

    int result = Catch::Session().run(argc, argv);

    // Clean up only after all tests run
    cleanUpGeneratedTestFiles();

    return result;
}

#include "core/PathRegistry.h"
#include <catch2/catch_session.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        PathRegistry::loadFromFile("config.json");
        PathRegistry::loadFromEnv();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Failed to initialize PathRegistry: " << ex.what() << std::endl;
        return 1;
    }

    Catch::Session session;
    return session.run(argc, argv);
}

#include "PathRegistry.h"
#include <cstdlib>
#include <fstream>
#include <json.hpp> // assumes you are using this library
#include <stdexcept>

std::unordered_map<std::string, std::string> PathRegistry::paths;

void PathRegistry::loadFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file)
    {
        throw std::runtime_error("Cannot open path config file: " + filepath);
    }
    nlohmann::json json;
    file >> json;
    for (auto& [key, value] : json.items())
    {
        paths[key] = value.get<std::string>();
    }
}

void PathRegistry::loadFromEnv()
{
    const char* env_working = std::getenv("DQUANT_WORKING_DIR");
    if (env_working)
    {
        paths["working_dir_cli"] = env_working;
    }
}

void PathRegistry::applyDefaults()
{
    if (paths.find("assets_dir") == paths.end())
    {
        paths["assets_dir"] = "assets/";
    }
    if (paths.find("working_dir_cli") == paths.end())
    {
        paths["working_dir_cli"] = "out/";
    }
    if (paths.find("working_dir_tests") == paths.end())
    {
        paths["working_dir_tests"] = "tests/test_output/";
    }
}

const std::string& PathRegistry::get(const std::string& key)
{
    if (paths.find(key) == paths.end())
    {
        throw std::runtime_error("Path key not found: " + key);
    }
    return paths.at(key);
}

#include "PathRegistry.h"
#include <cstdlib>
#include <fstream>
#include <json.hpp> // assumes you are using this library
#include <stdexcept>

#include <filesystem>
#include <fstream>

std::unordered_map<std::string, std::string> PathRegistry::paths;
std::filesystem::path PathRegistry::repoRootPath;


std::filesystem::path findConfigFileUpwards(const std::string& filename)
{
    namespace fs = std::filesystem;
    fs::path current = fs::current_path();

    while (true)
    {
        fs::path candidate = current / filename;
        if (fs::exists(candidate))
        {
            return candidate;
        }
        if (current == current.root_path())
        {
            throw std::runtime_error("Could not find config file: " + filename);
        }
        current = current.parent_path();
    }
}

void PathRegistry::loadFromFile(const std::string& filepath)
{
    auto foundFilePath = findConfigFileUpwards(filepath);
    repoRootPath = foundFilePath.parent_path();
    std::ifstream file(foundFilePath.string());
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

std::filesystem::path PathRegistry::getResolvedPath(const std::string& key)
{
    const auto& relPath = get(key);
    std::filesystem::path path(relPath);
    if (path.is_absolute())
    {
        return path;
    }
    return repoRootPath / path;
}


#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>

class PathRegistry
{
    public:
        // resolve all relative paths against this
        static std::filesystem::path repoRootPath;

        // Load JSON config file from disk
        static void loadFromFile(const std::string& filepath);

        // Optionally override with env variables like DQUANT_WORKING_DIR
        static void loadFromEnv();

        // Fetch resolved path (throws std::runtime_error if not found)
        static const std::string& get(const std::string& key);

        //
        static std::filesystem::path getResolvedPath(const std::string& key);

    private:
        static std::unordered_map<std::string, std::string> paths;
        static void applyDefaults();
};

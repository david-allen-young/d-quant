#pragma once
#include <cstdlib>
#include <string>

inline std::string env_or_fallback(const char* envKey, const std::string& fallback)
{
    const char* val = std::getenv(envKey);
    return val ? std::string(val) : fallback;
}

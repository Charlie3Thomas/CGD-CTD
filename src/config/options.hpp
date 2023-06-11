#pragma once

#include <cstddef>
#include <filesystem>

namespace CT
{
class ConfigSingleton
{
public:
    size_t image_width;
    size_t image_height;
    std::filesystem::path input_model_filename;
    std::filesystem::path image_filename;

    static ConfigSingleton& GetInstance();

    /// @brief Parse command line options
    /// @param argc 
    /// @param argv 
    static void ParseOptions(int argc, char** argv);

private:
    ConfigSingleton() = default;

    ConfigSingleton(const ConfigSingleton&) = delete;

    ConfigSingleton& operator=(const ConfigSingleton&) = delete;

    ConfigSingleton(ConfigSingleton&&) = delete;

    ConfigSingleton& operator=(ConfigSingleton&&) = delete;

    ~ConfigSingleton();

};
}
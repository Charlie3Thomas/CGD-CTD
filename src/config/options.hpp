#pragma once

#include "loaders/scene.hpp"

#include <cstddef>
#include <filesystem>

namespace CT
{
class ConfigSingleton
{
public:

    // Defined parameters
    Scene environment = double_dragon;
    std::filesystem::path image_filename;
    size_t image_width      = 1280;
    size_t image_height     = 720;
    size_t canvas_width     = 40;
    size_t canvas_height    = 40;
    size_t direct_samples   = 1;
    size_t indirect_samples = 1;
    size_t recursion_depth  = 1;
    bool   denoiser         = false;
    // Texture resolution
    // Adaptive material

    // Debug parameters
    bool use_bvh = false;
    bool visualise_canvases = false;
    bool visualise_normals = false;

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
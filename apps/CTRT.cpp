#include <iostream>
#include <limits>
#include <cassert>

#include <Eigen/Dense>

#include "bvh/bvh.hpp"
#include "camera/camera.hpp"
#include "camera/film.hpp"
#include "config/options.hpp"
#include "loaders/objloader.hpp"
#include "materials/material.hpp"
#include "renderers/testrenderer.hpp"
#include "utils/rgb.hpp"
#include "utils/exr.hpp"
#include "utils/ppm.hpp"
#include "utils/timer.hpp"
#include "utils/utils.hpp"

using namespace CT;

int main(int argc, char** argv)
{
    ConfigSingleton::ParseOptions(argc, argv);

    // Retrieve config singleton instance
    const ConfigSingleton& config = ConfigSingleton::GetInstance();

    // Load obj
    LoadObj();

    // Create film
    Film film(config.image_width, config.image_height, Eigen::Vector2i(39, 39));

    // Create camera
    Camera camera(
        Eigen::Vector3f(0.0F, 0.0F, 0.0F),   // Camera position
        Eigen::Vector3f(0.0F, 0.0F, 1.0F),   // Camera look direction
        Eigen::Vector3f(0.0F, 1.0F, 0.0F),   // Camera up direction
        1.0F);                               // Camera focal length

    std::unique_ptr<Renderer> renderer = std::make_unique<TestRenderer>();

    renderer->RenderFilm(film, camera);

    WriteToEXR(film.rgb.data(), config.image_width, config.image_height, config.image_filename.c_str());
    //WriteToEXR(film.GetPixelData().data(), static_cast<int>(width), static_cast<int>(height), config.image_filename.c_str());
    
    return EXIT_SUCCESS;
}

// Definitely next week
// TODO: Texture loading
// TODO: Multi-threading
    // 32x32 pixels per thread
    // Give each tile a unique ID
    // interlocked increment



// Maybe next week
// TODO: Diffuse materials
// TODO: Sampling functionality
    // solid angle & spherical coordinates
// TODO: Lighting
    // Point lights
    // Area lights
    

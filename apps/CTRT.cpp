#include <iostream>
#include <limits>
#include <cassert>

#include <Eigen/Dense>
#include <thread>

#include "bvh/bvh.hpp"
#include "camera/camera.hpp"
#include "camera/film.hpp"
#include "config/options.hpp"
#include "embree/embreesingleton.hpp"
#include "loaders/objloader.hpp"
//#include "loaders/object.hpp"
//#include "loaders/transform.hpp"
#include "loaders/scene.hpp"
//#include "materials/materials.hpp"
#include "renderers/testrenderer.hpp"
#include "utils/rgb.hpp"
#include "utils/exr.hpp"
#include "utils/ppm.hpp"
#include "utils/timer.hpp"
#include "utils/utils.hpp"

using namespace CT;
using namespace Eigen;

int main(int argc, char** argv)
{
    {
        Timer t = Timer("CTRT");

        ConfigSingleton::ParseOptions(argc, argv);

        // Retrieve config singleton instance
        const ConfigSingleton& config = ConfigSingleton::GetInstance();
/*
        EmbreeSingleton& embree = EmbreeSingleton::GetInstance();
        // Textures
        assert(!embree.textures.contains("water"));
        embree.textures.emplace("water", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/water.png")));
        assert(!embree.textures.contains("stone"));
        embree.textures.emplace("stone", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/stone.jpg")));
        assert(!embree.textures.contains("test"));
        embree.textures.emplace("test", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/capsule0.jpg")));
*/
        ObjectLoader loader = ObjectLoader();
        loader.LoadObjects(double_dragon.objects);

        if (ConfigSingleton::GetInstance().use_bvh){ BuildBVH(RTCBuildQuality::RTC_BUILD_QUALITY_LOW, loader.GetPrims(), nullptr, loader.GetPrims().size() * 2); }

        // Create film
        Film film(config.image_width, config.image_height, Eigen::Vector2i(config.canvas_width, config.canvas_height));

        // Create camera
        Camera camera(
            Vector3f(0.0F, 0.0F, 0.0F),   // Camera position
            Vector3f(0.0F, 0.0F, 1.0F),   // Camera look direction
            Vector3f(0.0F, 1.0F, 0.0F),   // Camera up direction
            1.0F);                        // Camera focal length

        // Render
        std::unique_ptr<Renderer> renderer0 = std::make_unique<TestRenderer>();
        renderer0->RenderFilm(film, camera, config.threads);

        // Write to .EXR file
        WriteToEXR(film.rgb.data(), config.image_width, config.image_height, config.image_filename.c_str());

        /*
            Use intel open denoiser
            Initialise denoiser
            Prepare input data
                - rendered image
                - albedo buffer
                - normal buffer
            Create denoising buffers
            Perform denoising
            Retrieve output data
            Write to file
            Cleanup resources
        */
    }    

    std::cout << std::endl;
    
    return EXIT_SUCCESS;
}
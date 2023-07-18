#include <iostream>
#include <limits>
#include <cassert>

#include <Eigen/Dense>
#include <thread>
#include "OpenImageDenoise/oidn.hpp"

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

        // Intel Open Image Denoise
        // Create an Intel Open Image Denoise device
        oidn::DeviceRef device = oidn::newDevice();
        device.commit();

        // Create buffers for input/output images accessible by both host (CPU) and device (CPU/GPU)
        oidn::BufferRef colour_buff = device.newBuffer(static_cast<size_t>(config.image_width * config.image_height * 3 * sizeof(float)));

        // Fill the input image buffers
        auto* colour_ptr = static_cast<float*>(film.rgb.data());
        std::memcpy(colour_buff.getData(), colour_ptr, config.image_width * config.image_height * 3 * sizeof(float));

        // Create a filter for denoising a beauty (colour) image using optional auxiliary images too
        // This can be an expensive operation, so try not to create a new filter for every image
        oidn::FilterRef filter = device.newFilter("RT");
        filter.setImage("color", colour_buff, oidn::Format::Float3, config.image_width, config.image_height); // Beauty
        // filter.setImage("albedo", albedo_buff, oidn::Format::Float3, config.image_width, config.image_height); // Albedo (optional)
        // filter.setImage("normal", normal_buff, oidn::Format::Float3, config.image_width, config.image_height); // Normal (optional)
        filter.setImage("output", colour_buff, oidn::Format::Float3, config.image_width, config.image_height); // Denoised beauty
        filter.set("hdr", true); // Signal that the images are HDR
        filter.commit();

        // Filter the beauty image
        filter.execute();

        // Check for errors
        const char* err_msg;
        if (device.getError(err_msg) != oidn::Error::None)
            std::cout << "OIDN Error: " << err_msg << std::endl;

        const auto* denoised_ptr = static_cast<const float*>(colour_buff.getData());

        // Write to .EXR file
        WriteToEXR(film.rgb.data(), config.image_width, config.image_height, config.image_filename.c_str());
        std::string denoised_filename = config.image_filename.string() + "_denoised.exr";
        WriteToEXR(denoised_ptr, config.image_width, config.image_height, denoised_filename.c_str());
    }    

    std::cout << std::endl;
    
    return EXIT_SUCCESS;
}
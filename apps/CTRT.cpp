#include <iostream>
#include <limits>
#include <cassert>

#include <Eigen/Dense>
#include <thread>
#include "OpenImageDenoise/oidn.hpp"
#include <tinyexr.h>

#include "bvh/bvh.hpp"
#include "camera/camera.hpp"
#include "camera/film.hpp"
#include "config/options.hpp"
#include "embree/embreesingleton.hpp"
#include "loaders/objloader.hpp"
#include "loaders/scene.hpp"
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
        ConfigSingleton& cs = ConfigSingleton::GetInstance();
        EmbreeSingleton& embree = EmbreeSingleton::GetInstance();

        // Textures
        assert(!embree.textures.contains("water"));
        embree.textures.emplace("water", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/water.png")));
        assert(!embree.textures.contains("stone"));
        embree.textures.emplace("stone", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/stone.jpg")));
        assert(!embree.textures.contains("test"));
        embree.textures.emplace("test", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/capsule0.jpg")));

        ObjectLoader loader = ObjectLoader();
        //loader.LoadObjects(double_dragon.objects);
        loader.LoadObjects(cs.environment.objects);

        if (ConfigSingleton::GetInstance().use_bvh){ BuildBVH(RTCBuildQuality::RTC_BUILD_QUALITY_LOW, loader.GetPrims(), nullptr, loader.GetPrims().size() * 2); }

        // Create film
        Film film(cs.image_width, cs.image_height, Eigen::Vector2i(cs.canvas_width, cs.canvas_height));

        Camera camera = cs.environment.camera;

        // Render
        std::unique_ptr<Renderer> renderer0 = std::make_unique<TestRenderer>();
        renderer0->RenderFilm(film, camera, std::thread::hardware_concurrency());
        //renderer0->RenderFilm(film, camera, 1);

        // Load the EXR image using the LoadEXR function from TinyEXR
        const char* filename = "/home/Charlie/CGD-CTD/ref/ref-cornell-box.exr";

        filename = "/home/Charlie/CGD-CTD/ref/ref-cornell-box.exr";

        float L1_diff;
        float L2_diff;
        int width = 0;
        int height = 0;
        float* rgba = LoadEXRFromFile(filename, width, height);

        if (ConfigSingleton::GetInstance().denoiser)
        {
            // Intel Open Image Denoise
            // Create an Intel Open Image Denoise device
            oidn::DeviceRef device = oidn::newDevice();
            device.commit();
    
            // Create buffers for input/output images accessible by both host (CPU) and device (CPU/GPU)
            oidn::BufferRef colour_buff = device.newBuffer(static_cast<size_t>(cs.image_width * cs.image_height * 3 * sizeof(float)));
            // oidn::BufferRef normal_buff = device.newBuffer(static_cast<size_t>(cs.image_width * cs.image_height * 3 * sizeof(float)));
    
            // Fill the input image buffers
            auto* colour_ptr = static_cast<float*>(film.rgb.data());
            std::memcpy(colour_buff.getData(), colour_ptr, cs.image_width * cs.image_height * 3 * sizeof(float));

            /* FOR SOME REASON WHEN PROVIDING OIDN WITH A NORMAL BUFFER, THE IMAGE DOES NOT GET PROCESSED AT ALL */

            //// Fill normal map buffer
            //const char* norms_filename = "/home/Charlie/CGD-CTD/normals/norm-cornell-box-normals.exr";
            //int w = 0;
            //int h = 0;
            //float* norms = LoadEXRFromFile(norms_filename, w, h);
            //std::memcpy(normal_buff.getData(), norms, cs.image_width * cs.image_height * 3 * sizeof(float));
    
            // Create a filter for denoising a beauty (colour) image using optional auxiliary images too
            // This can be an expensive operation, so try not to create a new filter for every image
            oidn::FilterRef filter = device.newFilter("RT");
            filter.setImage("color", colour_buff, oidn::Format::Float3, cs.image_width, cs.image_height); // Beauty
            // filter.setImage("albedo", albedo_buff, oidn::Format::Float3, config.image_width, config.image_height); // Albedo (optional)
            // filter.setImage("normal", normal_buff, oidn::Format::Float3, cs.image_width, cs.image_height); // Normal (optional)
            filter.setImage("output", colour_buff, oidn::Format::Float3, cs.image_width, cs.image_height); // Denoised beauty
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
            std::string denoised_filename = cs.image_filename.string();
            //WriteToEXR(denoised_ptr, cs.image_width, cs.image_height, denoised_filename.c_str());

            L1_diff = L1Difference(rgba, denoised_ptr, static_cast<size_t>(height), static_cast<size_t>(width));
            L2_diff = L2Difference(rgba, denoised_ptr, static_cast<size_t>(height), static_cast<size_t>(width));

            std::cout << "Denoised" << std::endl;
            std::cout << "L1 difference: " << L1_diff << std::endl;
            std::cout << "L2 difference: " << L2_diff << std::endl;
        }
        else
        {
            L1_diff = L1Difference(rgba, film.rgb.data(), static_cast<size_t>(height), static_cast<size_t>(width));
            L2_diff = L2Difference(rgba, film.rgb.data(), static_cast<size_t>(height), static_cast<size_t>(width));
            std::cout << "Raw" << std::endl;
            std::cout << "L1 difference: " << L1_diff << std::endl;
            std::cout << "L2 difference: " << L2_diff << std::endl;
            // WriteToEXR(film.rgb.data(), cs.image_width, cs.image_height, cs.image_filename.c_str());
        }
    }

    std::cout << std::endl;
    
    return EXIT_SUCCESS;
}
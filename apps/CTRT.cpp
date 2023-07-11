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
#include "loaders/object.hpp"
#include "loaders/transform.hpp"
#include "materials/mat.hpp"
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

        EmbreeSingleton& embree = EmbreeSingleton::GetInstance();

// PLACEHOLDER TEST

        // Materials
        assert(!embree.materials.contains("red"));
        embree.materials.emplace("red",   std::make_unique<Mat>(Mat{ RED   * 0.5F, RED   * 1.0F, 0.04F }));
        assert(!embree.materials.contains("blue")); 
        embree.materials.emplace("blue",  std::make_unique<Mat>(Mat{ BLUE  * 0.5F, BLUE  * 1.0F, 0.04F }));
        assert(!embree.materials.contains("green")); 
        embree.materials.emplace("green", std::make_unique<Mat>(Mat{ GREEN * 0.5F, GREEN * 1.0F, 0.04F }));
        assert(!embree.materials.contains("jade"));
        embree.materials.emplace("jade",  std::make_unique<Mat>(Mat
        {
            RGB{0.0F, 0.64F, 0.42F}  * 0.6F,  
            RGB{0.0F, 0.94F, 0.72F} * 0.5F, 
            0.95F,
            true
        }));
        assert(!embree.materials.contains("copper"));
        embree.materials.emplace("copper", std::make_unique<Mat>(Mat
        {
            RGB{0.64F, 0.20F, 0.08F}  * 0.6F, 
            RGB{0.94F, 0.72F, 0.21F} * 0.5F, 
            0.95F,
            true
        }));
        assert(!embree.materials.contains("silver"));
        embree.materials.emplace("silver", std::make_unique<Mat>(Mat{
            RGB{0.51F, 0.51F, 0.51F}  * 0.6F, 
            RGB{0.51F, 0.51F, 0.51F} * 0.5F, 
            0.95F,
            true
        }));
        assert(!embree.materials.contains("white_d"));
        embree.materials.emplace("white_d", std::make_unique<Mat>(Mat
        {
            RGB{1.00F, 1.00F, 1.00F}  * 0.6F, 
            RGB{1.00F, 1.00F, 1.00F}  * 0.5F, 
            0.1F,
            false
        }));
        assert(!embree.materials.contains("white_s"));
        embree.materials.emplace("white_s", std::make_unique<Mat>(Mat
        {
            RGB{1.00F, 1.00F, 1.00F}  * 0.6F, 
            RGB{1.00F, 1.00F, 1.00F}  * 0.5F, 
            0.95F,
            true
        }));

        // Textures
        assert(!embree.textures.contains("water"));
        embree.textures.emplace("water", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/water.png")));
        assert(!embree.textures.contains("stone"));
        embree.textures.emplace("stone", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/stone.jpg")));
        assert(!embree.textures.contains("test"));
        embree.textures.emplace("test", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/capsule0.jpg")));

        // Load objects
#if 1
        size_t num_objects = 1;
#else
        size_t num_objects = 5;
#endif

        std::vector<Object> objects;
        objects.reserve(num_objects);
        ObjectLoader loader = ObjectLoader();
        {   
            // Dragon obj path
            std::filesystem::path dragon = "/home/Charlie/CGD-CTD/obj/xyzrgb_dragon.obj";

            //Jade dragon
            assert(embree.materials.contains("jade"));
            const Mat* jade = embree.materials["jade"].get();
            objects.emplace_back(Object{dragon, 0.05F, Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 0.05F), Vector3f(-7.5F, 0.0F, 15.0F), jade, nullptr});

            // Copper dragon
            assert(embree.materials.contains("copper"));
            const Mat* copper = embree.materials["copper"].get();
            objects.emplace_back(Object{dragon, 0.05F, Matrix3f(MakeRotation(0.0F, 200.0F, 0.0F) * 0.05F), Vector3f(7.5F, 0.0F, 15.0F), copper, nullptr});

            // Stanford bunny
            std::filesystem::path bunny = "/home/Charlie/CGD-CTD/obj/stanford-bunny.obj";
            assert(embree.materials.contains("silver"));
            const Mat* silver = embree.materials["silver"].get();
            objects.emplace_back(Object{bunny, 7.5F, Matrix3f(MakeRotation(0.0F, 180.0F, 0.0F) * 7.5F), Vector3f(0.0F, -2.25F, 5.0F), silver, nullptr});

            // Planes
            assert(embree.materials.contains("white_d"));
            const Mat* white_d = embree.materials["white_d"].get();
            assert(embree.materials.contains("white_s"));
            const Mat* white_s = embree.materials["white_s"].get();
            std::filesystem::path plane = "/home/Charlie/CGD-CTD/obj/plane.obj";
            objects.emplace_back(Object{plane, 1000.0F, Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 1000.0F), Vector3f(0.0F, -2.0F, 10.0F), white_s, nullptr});
            objects.emplace_back(Object{plane, 1000.0F, Matrix3f(MakeRotation(90.0F, 0.0F, 0.0F) * 1000.0F), Vector3f(0.0F, 0.0F, 25.0F), white_d, nullptr});

            loader.LoadObjects(objects);
        }
// PLACEHOLDER TEST

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
    }    

    std::cout << std::endl;
    
    return EXIT_SUCCESS;
}

// Definitely next week
// TODO: Read about shading frames / shading bases
// TODO: Read about Gram-Schmidt orthogonalisation - specifically for rendering
// TODO: Light pure virtual base class
//          - Sample  light (intsensity), evaluate light and probability density function methods
//          - Point light (PDF = 1)
//          - Calculate direct lighting on a surface (visibility test) ? : take emitted light, multiply by BDRF, multiply by geometry term (dir to light . product of surface normal) / distance squared (r^2)


// Maybe next week
// TODO: Sampling functionality
    // solid angle & spherical coordinates
    

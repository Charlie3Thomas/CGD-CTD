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
#include "materials/material.hpp"
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
        embree.materials.emplace("red",   std::make_unique<Material>(RGB{1.0F, 0.0F, 0.0F},  1.0F, 0.5F, 0.5F));
        assert(!embree.materials.contains("blue"));
        embree.materials.emplace("blue",  std::make_unique<Material>(RGB{0.0F, 0.0F, 1.0F},  1.0F, 0.5F, 0.5F));
        assert(!embree.materials.contains("green"));
        embree.materials.emplace("green", std::make_unique<Material>(RGB{0.0F, 1.0F, 0.0F},  1.0F, 0.5F, 0.5F));

        // Textures
        assert(!embree.textures.contains("water"));
        embree.textures.emplace("water", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/water.png")));
        assert(!embree.textures.contains("stone"));
        embree.textures.emplace("stone", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/stone.jpg")));
        assert(!embree.textures.contains("test"));
        embree.textures.emplace("test", std::make_unique<Texture>(Texture("/home/Charlie/CGD-CTD/textures/capsule0.jpg")));

        // Load objects
        size_t num_objects = 1;

        std::vector<Object> objects;
        objects.reserve(num_objects);
        ObjectLoader loader = ObjectLoader();
        {
            float rotx = 0.0F;
            float roty = 0.0F;
            float rotz = 0.0F;
            float posx = -6.0F;
            for (size_t i = 0; i < num_objects; i++)
            {
                auto scale          = 1.0F;
                // auto transformation = Matrix3f   (MakeRotation(RandomRange(-360.0F, 360.0F), RandomRange(-360.0F, 360.0F), RandomRange(-360.0F, 360.0F)) * scale);
                // auto translation    = Vector3f   (RandomRange(-10.0F, 10.0F), RandomRange(-10.0F, 10.0F), RandomRange(10.0F, 50.0F));
                // auto transformation = Matrix3f   (MakeRotation(rotx, roty, rotz) * scale);
                // auto translation    = Vector3f   (posx, 0.0F, 10.0F);
                auto transformation = Matrix3f   (MakeRotation(45.0F, 45.0F, 45.0F) * scale);
                auto translation    = Vector3f   (0.0F, 0.0F, 10.0F);

                assert(embree.materials.contains("red"));
                const Material* mat = embree.materials["red"].get();

                const Texture* tex = nullptr;

                assert(embree.textures.contains("stone"));
                tex = embree.textures["stone"].get();

                // if (i % 2 == 0)
                // {

                // }   

                //objects.emplace_back(Object{config.input_model_filename, scale, transformation, translation, mat, tex});
                objects.emplace_back(Object{config.input_model_filename, scale, transformation, translation, mat, nullptr});

                rotx += 45.0F;
                roty += 45.0F;
                rotz += 45.0F;
                posx += 3.0F;
            }            
            loader.LoadObjects(objects);
        }
// PLACEHOLDER TEST

        if (ConfigSingleton::GetInstance().use_bvh){ BuildBVH(RTCBuildQuality::RTC_BUILD_QUALITY_LOW, loader.GetPrims(), nullptr, 1024); }

        // Create film
        Film film(config.image_width, config.image_height, Eigen::Vector2i(config.canvas_width, config.canvas_height));

        // Create camera
        Camera camera(
            Vector3f(0.0F, 0.0F, 0.0F),   // Camera position
            Vector3f(0.0F, 0.0F, 1.0F),   // Camera look direction
            Vector3f(0.0F, 1.0F, 0.0F),   // Camera up direction
            1.0F);                        // Camera focal length

        // Create renderer
        std::unique_ptr<Renderer> renderer = std::make_unique<TestRenderer>();
        
        // Render
        renderer->RenderFilm(film, camera, config.threads);
        
        //renderer->RenderFilmUnthreaded(film, camera);

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
    

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

        // Retrieve embree singleton instance
        EmbreeSingleton& embree = EmbreeSingleton::GetInstance();

        // BVH prims data
        std::vector<RTCBuildPrimitive> prims;

        // Model data cache
        std::vector<aiMesh> meshes;

        for (size_t i = 0; i < 500; i++)
        {
            auto scale              = RandomRange(0.05F, 0.5F);
            Matrix3f transformation = MakeRotation(RandomRange(0.0F, 360.0F),  RandomRange(0.0F, 360.0F),  RandomRange(0.0F, 360.0F)) * scale;
            Vector3f translation    =     Vector3f(RandomRange(-10.0F, 10.0F), RandomRange(-10.0F, 10.0F), RandomRange(10.0F, 100.0F));
            unsigned int geomID     = LoadObj(config.input_model_filename.c_str(), prims, transformation, translation);

            auto r = RandomRange(0.0F, 1.0F);
            auto g = RandomRange(0.0F, 1.0F);
            auto b = RandomRange(0.0F, 1.0F);

            embree.AddMaterial(geomID, Material{ RGB{r, g, b}, 1.0F, 0.5F, 0.5F });
        }        

        // // Obj transformation
        // float scale = 1.0F;
        // Matrix3f transformation = MakeRotation(0.0F, 0.0F, 0.0F) * scale;
        // Vector3f translation = Vector3f(-7.5F, 0.0F, 10.0F);

        // // Load obj
        // unsigned int geomID = LoadObj(config.input_model_filename.c_str(), prims, MakeRotation(0.0F, 0.0F, 0.0F) * 1.0F, Vector3f(-7.5F, 0.0F, 10.0F));
        // embree.AddMaterial(geomID, Material{ RGB{1.0F, 0.0F, 0.0F}, 0.5F, 0.5F, 0.5F });

        // geomID = LoadObj(config.input_model_filename.c_str(), prims, MakeRotation(0.0F, 180.0F, 0.0F) * 1.0F, Vector3f(7.5F, 0.0F, 10.0F));
        // embree.AddMaterial(geomID, Material{ RGB{0.0F, 1.0F, 0.0F}, 0.5F, 0.5F, 0.5F });

        // Create BVH
        if (ConfigSingleton::GetInstance().use_bvh){ BuildBVH(RTCBuildQuality::RTC_BUILD_QUALITY_HIGH, prims, nullptr, 1024); }

        

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
// TODO: Barycentric coordinates
// TODO: Texture loading
// TODO: Read about shading frames / shading bases
// TODO: Read about Gram-Schmidt orthogonalisation - specifically for rendering
// TODO: Light pure virtual base class
//          - Sample  light (intsensity), evaluate light and probability density function methods
//          - Point light (PDF = 1)
//          - Calculate direct lighting on a surface (visibility test) ? : take emitted light, multiply by BDRF, multiply by geometry term (dir to light . product of surface normal) / distance squared (r^2)


// Maybe next week
// TODO: Sampling functionality
    // solid angle & spherical coordinates
    

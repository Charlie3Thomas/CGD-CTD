#include <iostream>
#include <limits>
#include <cassert>

#include "bvh/bvh.hpp"
#include "camera/camera.hpp"
#include "config/options.hpp"
#include "utils/ppm.hpp"
#include "utils/exr.hpp"
#include "utils/timer.hpp"
#include "utils/utils.hpp"
#include "loaders/objloader.hpp"

using namespace CT;

bool Test()
{
    // Retrieve config singleton instance
    const ConfigSingleton& config = ConfigSingleton::GetInstance();

    // Load obj
    LoadObj();

    // Image size
    size_t width  = config.image_width;
    size_t height = config.image_height;

    // Float array for pixel data
    std::vector<float> rgb(width * height * 3);

    // Create camera
    Camera camera(
        Eigen::Vector3f(0.0F, 0.0F, 0.0F),   // Camera position
        Eigen::Vector3f(0.0F, 0.0F, 1.0F),   // Camera look direction
        Eigen::Vector3f(0.0F, 1.0F, 0.0F),   // Camera up direction
        1.0F);                               // Camera focal length

    //RTCRayHit ray (camera.FireRayAtPixel(horizontal, vertical));

    // Canvas size
    const Eigen::Vector2i canvas_size(width, height);

    int64_t total_time = 0;
    size_t calls = 0;

    int index = 0;
    for (size_t vertical = 0; vertical < height; vertical++)
    {
        for (size_t horizontal = 0; horizontal < width; horizontal++)
        {
            const Eigen::Vector2i pixel_index(horizontal, vertical);
        
            // Create ray
            RTCRayHit ray;
            {
                Timer t ([&total_time, &calls](int64_t time) { total_time += time; calls++; });
                ray = camera.GetRayForPixel(canvas_size, pixel_index);
            }           

            // default geomID to invalid
            ray.hit.geomID = RTC_INVALID_GEOMETRY_ID;

            // Create context
            RTCIntersectContext context; //TODO: get things
            rtcInitIntersectContext(&context);

            // Trace the ray against the scene
            rtcIntersect1(EmbreeSingleton::GetInstance().scene, &context, &ray);            

            if (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
            {
                //std::cout << ray.hit.Ng_x << ray.hit.Ng_y << ray.hit.Ng_z << std::endl;
                //Draw a colour depending on the normals
                rgb[3*index+0] = FromIntersectNormal(ray).r;
                rgb[3*index+1] = FromIntersectNormal(ray).g;
                rgb[3*index+2] = FromIntersectNormal(ray).b;
            }
            else
            {
                rgb[3*index+0] = 1.0F;
                rgb[3*index+1] = 1.0F;
                rgb[3*index+2] = 1.0F;
            }
           
           index++;
        }
    }

    WriteToEXR(rgb.data(), static_cast<int>(width), static_cast<int>(height), config.image_filename.c_str());
    
    return true;
}


int main(int argc, char** argv)
{
    ConfigSingleton::ParseOptions(argc, argv);

    Test();
    
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
    

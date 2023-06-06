#include <iostream>
#include <limits>
#include <cassert>

#include "bvh/bvh.hpp"
#include "config/options.hpp"
#include "utils/ppm.hpp"
#include "utils/exr.hpp"
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

    int half_width = static_cast<int>(width) / 2;
    int half_height = static_cast<int>(height) / 2;

    // Float array for pixel data
    std::vector<float> rgb(width * height * 3);

    // PPM body
    int index = 0;
    for (int vertical = half_height; vertical > -half_height; vertical--)
    {
        for (int horizontal = half_width; horizontal > -half_width; horizontal--)
        //for (int horizontal = -half_width; horizontal < half_width; horizontal++)
        {
            // Create ray
            RTCRayHit ray;
            ray.ray.org_x =  0.0F;
            ray.ray.org_y =  0.0F;
            ray.ray.org_z =  -10.0F;

            //TODO: Adjust angle of ray based on pixel position
            float pixel_width   = 1.0F / static_cast<float>(width);
            float pixel_height  = 1.0F / static_cast<float>(height);
            ray.ray.dir_x       = static_cast<float>(horizontal)  * pixel_width;
            ray.ray.dir_y       = static_cast<float>(vertical)    * pixel_height;
            ray.ray.dir_z       = 1.0F;

            ray.ray.tnear = 0.001F; // Set the minimum distance to start tracing
            ray.ray.tfar  = std::numeric_limits<float>::infinity();
            ray.ray.mask  = -1;

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
// TODO: Get BVH working
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
    

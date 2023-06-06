#include <iostream>
#include <limits>
#include <cassert>


//#include <embree3/rtcore.h>
//TODO: Find out where the hell you're supposed to get access to Vec3fa and BBox3fa from embree
// maybe use RTCPointQuery instead of Vec3fa?
// maybe use RTCBounds instead of BBox3fa?



#include "bvh/bvh.hpp"
#include "utils/ppm.hpp"
#include "utils/exr.hpp"
#include "utils/utils.hpp"
#include "loaders/objloader.hpp"

using namespace CT;

bool Test()
{
    // Load obj
    LoadObj();

    // Image size
    size_t width = 100;
    size_t height = 100;

    int half_width = static_cast<int>(width) / 2;
    int half_height = static_cast<int>(height) / 2;

    // Float array for pixel data
    float* rgb = new float[width * height * 3];

    // PPM body
    int index = 0;
    for (int vertical = half_height; vertical > -half_height; vertical--)
    {
        for (int horizontal = -half_width; horizontal < half_width; horizontal++)
        {
            // Create ray
            RTCRayHit ray;
            ray.ray.org_x =  0.0F;
            ray.ray.org_y =  0.0F;
            ray.ray.org_z =  -10.0F;

            //TODO: Adjust angle of ray based on pixel position
            float pixel_width  = 2.0F / static_cast<float>(width);
            float pixel_height = 2.0F / static_cast<float>(height);
            ray.ray.dir_x = static_cast<float>(horizontal)  * pixel_width;
            ray.ray.dir_y = static_cast<float>(vertical)    * pixel_height;
            ray.ray.dir_z = 1.0F;

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

            //std::cout << index << std::endl;

            if (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
            {
                rgb[3*index+0] = CT::RED.r;
                rgb[3*index+1] = CT::RED.g;
                rgb[3*index+2] = CT::RED.b;
            }
            else
            {
                rgb[3*index+0] = 0.0F;
                rgb[3*index+1] = 0.0F;
                rgb[3*index+2] = 0.0F;
            }

            

            // If ray hit write white to pixel

            // If ray missed write black to pixel
           
           index++;
        }
    }

    int w = static_cast<int>(width);
    int h = static_cast<int>(height);

    WriteToEXR(rgb, w, h, "/home/Charlie/CGD-CTD/output/output.exr");

    return true;
}

// "/home/Charlie/CGD-CTD/output/output.exr"

int main(int argc, char** argv)
{
    // const int width = 100;
    // const int height = 100;

    // // Gradient float array
    // float* rgb = new float[width * height * 3];
    // for (int i = 0; i < width * height; i++)
    // {
    //     float gradient = static_cast<float>(i) / static_cast<float>(width * height);

    //     rgb[3*i+0] = gradient;
    //     rgb[3*i+1] = gradient;
    //     rgb[3*i+2] = gradient;
    // }

    // WriteToEXR(rgb, width, height, "/home/Charlie/CGD-CTD/output/output.exr");
    Test();
    
    return EXIT_SUCCESS;
}

// Definitely next week
// TODO: Code restructure
// TODO: Get BVH working
// TODO: Texture loading
// TODO: Real saving functionality
    // PFM or EXR or HDR -> find a lbirary
    // Save data to a buffer
// TODO: Multi-threading
    // 32x32 pixels per thread
    // Give each tile a unique ID
    // interlocked increment
// TODO: 32bit float colour
    // Plugin to view
// TODO: Visualise the normals



// Maybe next week
// TODO: Diffuse materials
// TODO: Sampling functionality
    // solid angle & spherical coordinates
// TODO: Lighting
    // Point lights
    // Area lights
    

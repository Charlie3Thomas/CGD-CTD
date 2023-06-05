#include <iostream>
#include <limits>
#include <cassert>

//#include <embree3/rtcore.h>
//TODO: Find out where the hell you're supposed to get access to Vec3fa and BBox3fa from embree
// maybe use RTCPointQuery instead of Vec3fa?
// maybe use RTCBounds instead of BBox3fa?

#include "bvh/bvh.hpp"
#include "utils/ppm.hpp"
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

    //PPM header
    PPMWriteHeader(std::cout, width, height);

    int half_width = static_cast<int>(width) / 2;
    int half_height = static_cast<int>(height) / 2;  

    // PPM body
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

            // Determine a final colour for each pixel
            RGB final_colour = (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID ? CT::WHITE : CT::BLACK);

            // Write the pixel to the PPM file
            PPMWritePixel(std::cout, final_colour);
        }
    }    

    return true;
}

int main(int argc, char** argv)
{
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
    

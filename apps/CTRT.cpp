#include <iostream>
#include <limits>
#include <cassert>

#include <Eigen/Dense>

#include "bvh/bvh.hpp"
#include "camera/camera.hpp"
#include "camera/film.hpp"
#include "config/options.hpp"
#include "loaders/objloader.hpp"
#include "materials/material.hpp"
#include "utils/rgb.hpp"
#include "utils/exr.hpp"
#include "utils/ppm.hpp"
#include "utils/timer.hpp"
#include "utils/utils.hpp"

using namespace CT;

bool Test()
{
    // Retrieve config singleton instance
    const ConfigSingleton& config = ConfigSingleton::GetInstance();

    // Load obj
    LoadObj();

    // Specify material
    Material mat { RGB{1.0F, 0.0F, 0.0F}, 0.25F, 0.5F, 0.5F };

    // Image size
    size_t width  = config.image_width;
    size_t height = config.image_height;

    // Float array for pixel data
    std::vector<float> rgb(width * height * 3);

    // Canvas size
    const Eigen::Vector2i film_size(width, height);

    // Create camera
    Camera camera(
        Eigen::Vector3f(0.0F, 0.0F, 0.0F),   // Camera position
        Eigen::Vector3f(0.0F, 0.0F, 1.0F),   // Camera look direction
        Eigen::Vector3f(0.0F, 1.0F, 0.0F),   // Camera up direction
        1.0F);                               // Camera focal length          

    // Create Film
    Film film(width, height, 1);


    // Light direction
    Eigen::Vector3f light_dir(-1.0F, 1.0F, -1.0F);
    light_dir.normalize();

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
                ray = camera.GetRayForPixel(film_size, pixel_index);
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
                Eigen::Vector3f hit_normal(ray.hit.Ng_x, ray.hit.Ng_y, ray.hit.Ng_z);
                hit_normal.normalize();

                float cos_theta = std::max(0.0F, std::abs(hit_normal.dot(light_dir)));

                Eigen::Vector3f raydir = Eigen::Vector3f(ray.ray.dir_x, ray.ray.dir_y, ray.ray.dir_z);

                // Calculate reflected ray direction
                Eigen::Vector3f reflection = (2.0F * hit_normal - raydir);
                reflection.normalize();

                float scale = -hit_normal.dot(raydir) * mat.intensity * std::abs(light_dir.dot(reflection));

                float r = mat.base_colour.r * scale * cos_theta;
                float g = mat.base_colour.g * scale * cos_theta;
                float b = mat.base_colour.b * scale * cos_theta;

                film.WritePixelDataToCanvas(r, g, b, 0, index);


                rgb[3*index+0] = mat.base_colour.r * scale * cos_theta;
                rgb[3*index+1] = mat.base_colour.g * scale * cos_theta;
                rgb[3*index+2] = mat.base_colour.b * scale * cos_theta;

                // //Draw a colour depending on the normals
                // rgb[3*index+0] = FromIntersectNormal(ray).r;
                // rgb[3*index+1] = FromIntersectNormal(ray).g;
                // rgb[3*index+2] = FromIntersectNormal(ray).b;
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

    //WriteToEXR(rgb.data(), static_cast<int>(width), static_cast<int>(height), config.image_filename.c_str());
    WriteToEXR(film.GetPixelData().data(), static_cast<int>(width), static_cast<int>(height), config.image_filename.c_str());
    
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
    

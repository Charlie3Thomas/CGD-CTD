#include <iostream>
#include <thread>
#include <FreeImage.h>
#include "ppm.hpp"

// TODO rays
// TODO bounding volumes
// TODO BVH
// TODO intersection
// TODO parameters
// TODO primative shapes
// TODO camera
// TODO lights
// TODO materials
// TODO scenes
// TODO .obj loader
// TODO textures with libfreeimage

int main() 
{

    size_t h = 1920;
    size_t v = 1080;

    ppm_write_header(std::cout, h, v);

    for(size_t i = 0; i < h*v; i++)
    {
        ppm_write_pixel(std::cout, rgb8_t { 255, 0, 0 });
    }

    return EXIT_SUCCESS;
}
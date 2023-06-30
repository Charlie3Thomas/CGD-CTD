#pragma once

#include "utils/rgb.hpp"

namespace CT
{
/// @brief Basic material specifying base colour, intensity, roughness and albedo
struct Material
{
    // Base colour
    RGB base_colour;

    // Intensity
    float   intensity;
    
    // Roughness
    float   roughness;

    // Albedo
    float   albedo;  
};



}





#pragma once

#include "utils/rgb.hpp"

/// @brief Basic material specifying base colour, intensity, roughness and albedo
struct Material
{
    // Base colour
    CT::RGB base_colour;

    // Intensity
    float   intensity;
    
    // Roughness
    float   roughness;

    // Albedo
    float   albedo;   
};
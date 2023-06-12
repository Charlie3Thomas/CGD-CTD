#pragma once

#include "utils/rgb.hpp"

namespace CT
{
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
}

// TODO: Incoming and outgoing light directions -> Evaluate BRDF
// TODO: Given an outgoing dir, sample incoming dir -> Sample BRDF
// TODO: Given incoming and outgoing light dirs, -> Probability density function
//       -

// TODO: Material base class
//          -evaluate sample (colour / pi)
//          -calculate PDF (probability density function)
//          -sample BRDF (sample incoming light direction given outgoing light direction)
//          -cosin weighted hemesphere sampling



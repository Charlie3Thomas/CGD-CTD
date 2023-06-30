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

    // virtual RGB Evaluate(const Vector3f& incident, const Vector3f& normal, const Vector3f& outgoing) const = 0;

    // virtual float PDF(const Vector3f& incident, const Vector3f& normal, const Vector3f& outgoing) const = 0;

    // virtual Vector3f SampleBDRF(const Vector3f& incident, const Vector3f& normal) const = 0;

    // virtual Vector3f CosineWeightedHemisphereSample(const Vector3f& normal) const = 0; 
};



}





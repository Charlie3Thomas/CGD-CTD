#pragma once

#include "utils/rgb.hpp"

#include <Eigen/Dense>

#include <array>

using namespace Eigen;

namespace CT
{
/// @brief The base class for all lights. Also acts as an ambient light.
struct Light
{   
    Light(const RGB& col, float intensity) : intensity(intensity) { colour[0] = col.r; colour[1] = col.g; colour[2] = col.b; }
    std::array<float, 3> colour;
    float intensity;
};

/// @brief A directional light.
struct DirectionalLight : public Light
{
    DirectionalLight(const RGB& col, float intensity, const Vector3f& dir) : Light(col, intensity), direction(std::move(dir)) {}
    Vector3f direction;
};

/// @brief A point light.
struct PointLight : public Light
{
    PointLight(const RGB& col, float intensity, const Vector3f& pos, float att) : Light(col, intensity), position(std::move(pos)), attenuation(att) {}
    Vector3f position;
    float attenuation;
};

}
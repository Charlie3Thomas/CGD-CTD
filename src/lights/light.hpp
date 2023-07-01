#pragma once

#include "utils/rgb.hpp"

#include <Eigen/Dense>

#include <array>

using namespace Eigen;

namespace CT
{
struct AmbientLight
{   
    RGB colour;
};

struct DirectionalLight
{
    RGB colour;
    Vector3f direction;
};

struct PointLight
{
    RGB colour;
    Vector3f position;

    // constant
    float attenuation;

    // linear
    // float linear attenuation;

    // quadratic
    // float quadratic attenuation;
};

struct Lights
{
    std::vector<AmbientLight> ambient;
    std::vector<DirectionalLight> directional;
    std::vector<PointLight> point;
};

}
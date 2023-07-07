#pragma once

#include "utils/rgb.hpp"

#include <Eigen/Dense>

#include <array>

namespace CT
{
struct AmbientLight
{   
    RGB colour;
};

struct DirectionalLight
{
    RGB colour;
    Eigen::Vector3f direction;
};

struct PointLight
{
    RGB colour;
    Eigen::Vector3f position;

    // constant
    float attenuation;

    // linear
    // float linear attenuation;

    // quadratic
    // float quadratic attenuation;
};

struct Lights
{
public:
    std::vector<AmbientLight> ambient;
    std::vector<DirectionalLight> directional;
    std::vector<PointLight> point;
};

}
#pragma once

#include <vector>

#include "shapes/sphere.hpp"

namespace CT
{
struct Scene
{
    Scene(const std::vector<Sphere>& sp) : spheres(sp) {}

    std::vector<Sphere> spheres;
};
}
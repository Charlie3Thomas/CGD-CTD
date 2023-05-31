#pragma once

#include "object.hpp"

#include "lights/lights.hpp"

#include <vector>

namespace scene
{

struct scene
{
    std::vector<object> objects;

    std::vector<lights::ambient_light> ambient_lights;
    std::vector<lights::point_light> point_lights;
    std::vector<lights::directional_light> directional_lights;
};

}
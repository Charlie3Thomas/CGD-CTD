#pragma once

#include "camera.hpp"

namespace cameras
{
    
struct pinhole : camera
{
    spacial_t position;
    spacial_t viewdir;
    spacial_t updir;
    double    aspectratio;
    double    fov;

    ray_t generate_ray(std::size_t hori, std::size_t vert) const override;
};

}
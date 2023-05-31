#pragma once

#include "common.hpp"

namespace cameras
{

struct camera
{
    virtual ~camera() = default;

    struct film
    {
        std::size_t horizontal_pixels;
        std::size_t vertical_pixels;
    };
    film f;

    virtual ray_t generate_ray(std::size_t hori, std::size_t vert) const = 0;
};

}
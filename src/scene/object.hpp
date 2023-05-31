#pragma once

#include <memory>

#include "materials/material.hpp"
#include "shapes/shape.hpp"

#include "common.hpp"

namespace scene
{

struct object
{
    std::unique_ptr<shapes::shape> shp;
    spacial_t pos = { 0, 0, 0 };
    transform_t trans = math::identity<double, 3>();
    materials::material mat;
};

}
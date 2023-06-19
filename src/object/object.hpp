#pragma once

#include "textures/texture.hpp"
#include "materials/material.hpp"

#include <embree3/rtcore.h>

#include <cstdint>

namespace CT
{
class Object
{
public:
    Object(RTCGeometry& geom, Material& mat, Texture& tex);
    ~Object();

    // Has an embree object
    RTCGeometry geom;

    // Has a material
    Material material;

    // Has a texture
    Texture texture;

    // Object ID
    uint64_t id;

};
}
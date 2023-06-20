#pragma once

#include "textures/texture.hpp"

#include <cstdint>
#include <iostream>
#include <cmath>
#include <vector>
#include <embree3/rtcore.h>

namespace CT
{
/// @brief A type containing the red, green, and blue values of a pixel.
struct RGB
{
    float r;
    float g;
    float b;
};

constexpr RGB RED       {255.0F,     0,         0.0F};
constexpr RGB GREEN     {  0.0F,   255.0F,      0.0F};
constexpr RGB BLUE      {  0.0F,     0.0F,    255.0F};
constexpr RGB YELLOW    {255.0F,   255.0F,      0.0F};
constexpr RGB ORANGE    {255.0F,   165.0F,      0.0F};
constexpr RGB PURPLE    {160.0F,    32.0F,    240.0F};
constexpr RGB BLACK     {  0.0F,    0.0F,       0.0F};
constexpr RGB WHITE     {255.0F,   255.0F,    255.0F};

inline RGB FromIntersectNormal(RTCRayHit rayhit)
{
    return 
    {
        rayhit.hit.Ng_x * 100, 
        rayhit.hit.Ng_y * 100, 
        rayhit.hit.Ng_z * 100
    };
}

inline RGB FromBaryCoords(RTCRayHit rayhit)
{
    return 
    {
        rayhit.hit.u,
        rayhit.hit.v,
        1.0F - rayhit.hit.u - rayhit.hit.v
    };
}

inline RGB FromTexture(RTCRayHit rayhit, Texture& tex)
{
    // Calculate the texture coordinates scaled by texture dimensions
    float u = rayhit.hit.u * tex.width;
    float v = rayhit.hit.v * tex.width;

    int x = static_cast<int>(u);
    int y = static_cast<int>(v);

    int texel_offset = static_cast<int>((x + y * tex.width) * 3);

    BYTE b = tex.buffer[texel_offset];
    BYTE g = tex.buffer[texel_offset + 1];
    BYTE r = tex.buffer[texel_offset + 2];

    return 
    { 
        r / 255.0F, 
        g / 255.0F, 
        b / 255.0F 
    };
}
}


inline std::ostream& operator<<(std::ostream& os, const CT::RGB& rgb)
{
    return os << static_cast<int>(rgb.r) << ' ' << static_cast<int>(rgb.g) << ' ' << static_cast<int>(rgb.b);
}




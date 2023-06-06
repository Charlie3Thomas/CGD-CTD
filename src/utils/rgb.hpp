#pragma once

#include <cstdint>
#include <iostream>
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

RGB FromIntersectNormal(RTCRayHit rayhit)
{
    return 
    {
        rayhit.hit.Ng_x * 100, 
        rayhit.hit.Ng_y * 100, 
        rayhit.hit.Ng_z * 100
    };
}
}

inline std::ostream& operator<<(std::ostream& os, const CT::RGB& rgb)
{
    return os << static_cast<int>(rgb.r) << ' ' << static_cast<int>(rgb.g) << ' ' << static_cast<int>(rgb.b);
}




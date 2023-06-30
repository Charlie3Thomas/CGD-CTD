#pragma once

#include "textures/texture.hpp"
#include "loaders/prims.hpp"

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

    RGB operator*(float scalar) const { return {r * scalar, g * scalar, b * scalar}; }
    RGB operator*=(float scalar) { r *= scalar; g *= scalar; b *= scalar; return *this; }
    RGB operator +=(const RGB& other) { r += other.r; g += other.g; b += other.b; return *this; }
    RGB operator +(const RGB& other) const { return {r + other.r, g + other.g, b + other.b}; }
    RGB operator / (float scalar) const { return {r / scalar, g / scalar, b / scalar}; }
};

constexpr RGB RED       {1.00F, 0.000f, 0.000F};
constexpr RGB GREEN     {0.00F, 1.000F, 0.000F};
constexpr RGB BLUE      {0.00F, 0.000F, 1.000F};
constexpr RGB YELLOW    {1.00F, 1.000F, 0.000F};
constexpr RGB ORANGE    {1.00F, 0.647F, 0.000F};
constexpr RGB PURPLE    {0.63F, 0.125F, 0.940F};
constexpr RGB BLACK     {0.00F, 0.000F, 0.000F};
constexpr RGB WHITE     {1.00F, 1.000F, 1.000F};

RGB FromIntersectNormal(RTCHit& hit);
RGB FromNormal(Vector3f flimbo);
RGB FromBaryCoords(RTCHit& hit);
RGB FromTexture(RTCHit& hit, const Texture* tex, const UVTextureCoords& tex_coords);

}


inline std::ostream& operator<<(std::ostream& os, const CT::RGB& rgb)
{
    return os << static_cast<int>(rgb.r) << ' ' << static_cast<int>(rgb.g) << ' ' << static_cast<int>(rgb.b);
}





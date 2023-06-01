#pragma once

#include <cstdint>
#include <iostream>

namespace CT
{
/// @brief A type containing the red, green, and blue values of a pixel.
struct RGB
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
};

constexpr RGB RED   {255,     0,      0};
constexpr RGB GREEN {  0,   255,      0};
constexpr RGB BLUE  {  0,     0,    255};
}

inline std::ostream& operator<<(std::ostream& os, const CT::RGB& rgb)
{
    return os << static_cast<int>(rgb.r) << ' ' << static_cast<int>(rgb.g) << ' ' << static_cast<int>(rgb.b);
}
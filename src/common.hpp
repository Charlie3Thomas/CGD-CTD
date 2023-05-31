#pragma once

#include "math/line.hpp"
#include "math/matrix.hpp"

// Some useful geometric typedefs 
using ray_t       = math::line<double, 3>;
using vector3_t   = math::vector<double, 3>;
using transform_t = math::matrix<double, 3, 3>;

// We define our bounding volumes with this
struct boundary
{
    vector3_t min;
    vector3_t max;
};

using colour_t = math::vector<double, 3>;

// The actual RBG values of the pixels. We template for optional 8-bit / 16-bit colour
template <typename T = std::uint8_t>
requires std::same_as<T, std::uint8_t> || std::same_as<T, std::uint16_t>
using rgb_t = math::vector<T, 3>;

using rgb8_t  = rgb_t<std::uint8_t>;
using rgb16_t = rgb_t<std::uint16_t>;

// Converts a floating point colour to its corresponding RGB value (values over 1 are truncated)
template <typename T = std::uint8_t>
constexpr rgb_t<T> to_rgb(const colour_t& colour)
{
    rgb_t<T> ret;
    for (std::size_t i = 0; i < 3; i++)
        ret[i] = std::round(std::min(std::abs(colour[i]), 1.0) * std::numeric_limits<T>::max());
    return ret;
}
#include "disk.hpp"

namespace shapes
{

bool disk::hit(const ray_t& ray) const
{
    // Not really any saving to be done here...
    intersection intersec;
    return hit(ray, intersec);
}

bool disk::hit(const ray_t& ray, intersection& intersec) const
{
    // Returns false immediately if the ray doesn't even hit the z = 0 plane or we start inside it
    constexpr auto epsilon = std::numeric_limits<double>::epsilon();
    if (std::abs(ray.origin[2]) < epsilon || std::abs(ray.direction[2]) < epsilon)
        return false;

    // Return if it was behind the origin
    if (std::signbit(ray.origin[2]) == std::signbit(ray.direction[2]))
        return false;

    // Calculate the intersection point and return whether it lies within the unit circle
    const auto t = -ray.origin[2] / ray.direction[2];
    if (std::pow(ray[t][0], 2) + std::pow(ray[t][1], 2) > 1)
        return false;
    
    intersec.t = t;
    intersec.n = std::signbit(ray.origin[2]) ? spacial_t({ 0, 0, -1 }) : spacial_t({ 0, 0, 1});
    return true;
}

boundary disk::get_boundary() const
{
    return { .min = { -1, -1, 0 }, .max = { 1, 1, 0 } };
}

}
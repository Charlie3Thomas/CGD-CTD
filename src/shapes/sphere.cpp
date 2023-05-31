#include "sphere.hpp"

namespace shapes
{

bool sphere::hit(const ray_t& ray) const
{
    // Computes whether a simultaneous solution to r = o + td and r.r = 1 exists
    const auto b = 2 * math::dot_product(ray.origin, ray.direction);
    const auto c = math::square_length(ray.origin) - 1;
    const auto d = std::pow(b, 2) - 4*c;
    
    if (d < std::numeric_limits<double>::epsilon())
        return false;

    // Makes sure the intersection isn't behind the rays origin - we do this without computing the expensive square-root of d
    if (b > 0 && std::pow(b, 2) > d)
        return false;

    return true;
}

bool sphere::hit(const ray_t& ray, intersection& intersec) const
{
    // Computes whether a simultaneous solution to r = o + td and r.r = 1 exists
    const auto b = 2 * math::dot_product(ray.origin, ray.direction);
    const auto c = math::square_length(ray.origin) - 1;
    const auto d = std::pow(b, 2) - 4*c;

    if (d < std::numeric_limits<double>::epsilon())
        return false;

    // Makes sure the intersection isn't behind the rays origin - we do this without computing the expensive square-root of d
    if (b > 0 && std::pow(b, 2) > d)
        return false;

    // Where on the line did we intersect
    const auto r = std::sqrt(d), x = -b - r, y = -b + r;
    const auto t = (x > 0) ? x/2 : y/2;

    // Fill out the intersection struct
    intersec.t = t;
    intersec.n = ray[t];

    return true;
}

boundary sphere::get_boundary() const
{
    return { .min = { -1, -1, -1 }, .max = { 1, 1, 1 } };
}

}
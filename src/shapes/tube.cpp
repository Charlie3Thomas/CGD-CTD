#include "tube.hpp"

namespace shapes
{

bool tube::hit(const ray_t& ray) const
{
    // Not really any saving to be done here...
    intersection intersec;
    return hit(ray, intersec);
}

bool tube::hit(const ray_t& ray, intersection& intersec) const
{
    // Return false if the rays are vertical
    constexpr auto epsilon = std::numeric_limits<double>::epsilon();
    if (std::abs(ray.direction[0]) < epsilon && std::abs(ray.direction[0]) < epsilon)
        return false;

    // Returns false if there is no solution to x^2 + y^2 = 1
    const auto a = std::pow(ray.direction[0], 2) + std::pow(ray.direction[1], 2);
    const auto b = 2 * (ray.origin[0] * ray.direction[0] + ray.origin[1] * ray.direction[1]);
    const auto c = std::pow(ray.origin[0], 2) + std::pow(ray.origin[1], 2) - 1;
    const auto d = std::pow(b, 2) - 4*a*c;

    if (d < epsilon)
        return false;
    
    // Makes sure the intersection isn't behind the rays origin - we do this without computing the expensive square-root of Δ
    if (b > 0 && std::pow(b, 2) > d)
        return false;
        
    // Compute the hitting points - this is inevitable at this stage 
    const auto tainv = 1 / (2 * a);
    const auto r = std::sqrt(d), x = (-b - r) * tainv, y = (-b + r) * tainv;
    const auto p = ray[x], q = ray[y];

    // We are potentially hitting the outside of the tube
    if (x > 0 && -1 < p[2] && p[2] < 1)
    {
        intersec.t = x;
        intersec.n = { p[0], p[1], 0 };
        return true;
    }

    // We are hitting the inside of the tube
    if (-1 < q[2] && q[2] < 1)
    {
        intersec.t = y;
        intersec.n = { -q[0], -q[1], 0 };
        return true;
    }
    
    return false;
}

boundary tube::get_boundary() const
{
    return { .min = { -1, -1, -1 }, .max = { 1, 1, 1 } };
}

}
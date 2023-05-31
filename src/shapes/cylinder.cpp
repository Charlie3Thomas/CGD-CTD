#include "cylinder.hpp"

#include "disk.hpp"
#include "tube.hpp"

namespace shapes
{

bool cylinder::hit(const ray_t& ray) const
{
    // Do we hit the tube or two disks?
    tube t;
    disk d;
    return t.hit(ray) || d.hit({ .origin = ray.origin + spacial_t({ 0, 0, 1 }), .direction = ray.direction }) || d.hit({ .origin = ray.origin - spacial_t({ 0, 0, 1 }), .direction = ray.direction });
}

bool cylinder::hit(const ray_t& ray, intersection& intersec) const
{
    bool ret = false;

    tube t;
    disk d;
    intersection partial;

    // Intersect the tube
    if (t.hit(ray, partial) && (!ret || partial.t < intersec.t))
    {
        ret = true;
        intersec = partial;
    }
    
    // Intersect either disk
    if (d.hit({ .origin = ray.origin + spacial_t({ 0, 0, 1 }), .direction = ray.direction }, partial) && (!ret || partial.t < intersec.t))
    {
        ret = true;
        intersec = partial;
    }
    if (d.hit({ .origin = ray.origin - spacial_t({ 0, 0, 1 }), .direction = ray.direction }, partial) && (!ret || partial.t < intersec.t))
    {
        ret = true;
        intersec = partial;
    }

    return ret;
}

boundary cylinder::get_boundary() const
{
    return { .min = { -1, -1, -1 }, .max = { 1, 1, 1 } };
}

}
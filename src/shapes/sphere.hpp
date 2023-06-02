#pragma once

#include <CGAL/Exact_spherical_kernel_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Sphere_3.h>

using Cartesian_k   = CGAL::Linear_k1;
using Sphere_3      = CGAL::Sphere_3<Cartesian_k>;
using CTSphere      = CGAL::Sphere_3<Cartesian_k>;

namespace CT
{
class Sphere : public Sphere_3
{
    
public:
    using CTSphere::CTSphere;
    Sphere(const Sphere_3& sphere, const RGB& color) : Sphere_3(sphere), colour(color) {}
    RGB colour;
    
};
}

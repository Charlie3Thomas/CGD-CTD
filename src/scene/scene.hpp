#pragma once

#include <vector>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_spherical_kernel_3.h>

using Cartesian_k   = CGAL::Linear_k1;
using Sphere_3      = CGAL::Sphere_3<Cartesian_k>;

namespace CT
{
struct Scene
{
    Scene(std::vector<Sphere_3> spheres) : spheres(spheres) { }

    std::vector<Sphere_3> spheres;
};
}
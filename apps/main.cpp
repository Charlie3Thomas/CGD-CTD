#include <iostream>
#include <CGAL/Exact_spherical_kernel_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Ray_3.h>
// #include <CGAL/intersections.h>
// #include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include "utils/ppm.hpp"

using namespace CT;

using Cartesian_k   = CGAL::Linear_k1;
using Point_3       = CGAL::Point_3<Cartesian_k>;
using Sphere_3      = CGAL::Sphere_3<Cartesian_k>;
using Ray_3         = CGAL::Ray_3<Cartesian_k>;  
using Direction_3   = CGAL::Direction_3<Cartesian_k>;

int main()
{
    Sphere_3 sphere = Sphere_3(Point_3(0, 0, 0), 100);

    size_t width = 100;
    size_t height = 100;

    PPMWriteHeader(std::cout, width, height);

    for (int vertical = -50; vertical < 50; vertical++)
    {
        for (int horizontal = -50; horizontal < 50; horizontal++)
        {
            Ray_3 ray = Ray_3(Point_3(-5, horizontal, vertical), Direction_3(1, 0, 0));

            PPMWritePixel(std::cout, CGAL::do_intersect(sphere, ray) ? CT::GREEN : CT::RED);
        }
    }

    return EXIT_SUCCESS;
}
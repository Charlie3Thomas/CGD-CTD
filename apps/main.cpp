#include <iostream>
#include <random>

#include <CGAL/Exact_spherical_kernel_3.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Sphere_3.h>
#include <CGAL/Ray_3.h>

#include "utils/ppm.hpp"
#include "utils/utils.hpp"
#include "scene/scene.hpp"
#include "shapes/sphere.hpp"

using namespace CT;

using Cartesian_k   = CGAL::Linear_k1;
using Point_3       = CGAL::Point_3<Cartesian_k>;
using Sphere_3      = CGAL::Sphere_3<Cartesian_k>;
using Ray_3         = CGAL::Ray_3<Cartesian_k>;  
using Direction_3   = CGAL::Direction_3<Cartesian_k>;

int main()
{
    std::vector<Sphere> obj;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-20, 20);
    for (int i = 0; i < 10; i++)
    {
        Point_3 p = Point_3(0, dis(gen), dis(gen));

        Sphere s = Sphere(Sphere_3(p, 100), RandomColour());
        obj.push_back(s);
    }

    Scene scene = Scene(obj);

    size_t width = 100;
    size_t height = 100;
    int half_width = static_cast<int>(width) / 2;
    int half_height = static_cast<int>(height) / 2;

    PPMWriteHeader(std::cout, width, height);

    for (int vertical = -half_height; vertical < half_height; vertical++)
    {
        for (int horizontal = -half_width; horizontal < half_width; horizontal++)
        {
            Ray_3 ray = Ray_3(Point_3(-5, horizontal, vertical), Direction_3(1, 0, 0));

            RGB final_colour = CT::BLACK; 

            for (size_t i = 0; i < scene.spheres.size(); i++)
                final_colour = CGAL::do_intersect(scene.spheres[i], ray) ? scene.spheres[i].colour : final_colour;

            PPMWritePixel(std::cout, final_colour);
        }
    }

    return EXIT_SUCCESS;
}
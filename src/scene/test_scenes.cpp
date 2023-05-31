#include "test_scenes.hpp"

#include "shapes/sphere.hpp"
#include "shapes/cylinder.hpp"
#include "cameras/digital.hpp"
#include "materials/test_materials.hpp"

namespace scene
{

static scene make_single_sphere()
{
    scene ret;

    // Camera
    auto cam = std::make_unique<cameras::digital>();

    // Object
    ret.objects.emplace_back(object{
        .shp = std::make_unique<shapes::sphere>(),
        .pos = { 5, 0, 0 },
        .trans = { 
            1, 0, 0,
            3, 5, 0,
            0, 2, 1
        },
        .mat = materials::matt_white
    });

    // Lighting
    ret.ambient_lights.push_back({ 0, 0.1, 0.1 });

    return ret;
}

static scene make_cylinders()
{
    scene ret;

    auto cam = std::make_unique<cameras::digital>();

    ret.objects.emplace_back(object{
        .shp = std::make_unique<shapes::cylinder>(),
        .pos = { 8, -8, 0 },
        .mat = materials::matt_white
    });

    for (std::size_t i = 0; i < 5; i++)
    {
        const auto& back = ret.objects.back();
        ret.objects.emplace_back(object({
            .shp   = back.shp->clone(),
            .pos   = back.pos + spacial_t({ 0, 3, 0 }),
            .trans = back.trans * math::rotation<double>(0.5, 0.5, 0.5),
            .mat   = back.mat
        }));
    }

    ret.ambient_lights.push_back({ 0, 0.1, 0.1 });

    return ret;
}

const scene single_sphere = make_single_sphere();
const scene cylinders     = make_cylinders();

}
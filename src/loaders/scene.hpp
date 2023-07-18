#pragma once

#include "lights/light.hpp"
#include "loaders/object.hpp"
#include "loaders/transform.hpp"

namespace CT
{
struct Scene
{
    Lights lights;
    std::vector<Object> objects;
};

inline Scene double_dragon
{
    /* lights */
    {
        /* directional */
        {
            /* directional light 1 */
            { .colour = WHITE  * 0.4F,  .direction = Eigen::Vector3f{ 1.0F, 0.1F, 0.0F } },
            /* directional light 2 */
            { .colour = WHITE  * 0.4F,  .direction = Eigen::Vector3f{ -1.0F, 0.1F, 0.0F } }
        },
        /* point */
        {
            /* point light 1 */
            { .colour = WHITE * 100.0F, .position  = Eigen::Vector3f{ 0.0F, 10.0F, 9.0F } },
            /* point light 2 */
            { /* point light properties */ }
        },
        /* area_cuboid */
        {
            /* area light cuboid 1 */
            { /* area cuboid properties */ }
        },
        /* area_sphere */
        {
            /* area light sphere 1 */
            { /* area sphere properties */ }
        }
    },
    /* objects */
    {
        /* object 1 */
        { "/home/Charlie/CGD-CTD/obj/xyzrgb_dragon.obj",  0.05F,   Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F)   * 0.05F),   Eigen::Vector3f(-7.5F, 0.0F, 15.0F), EmbreeSingleton::GetInstance().materials["jade"].get(),    nullptr },
        /* object 2 */     
        { "/home/Charlie/CGD-CTD/obj/xyzrgb_dragon.obj",  0.05F,   Eigen::Matrix3f(MakeRotation(0.0F, 200.0F, 0.0F) * 0.05F),   Eigen::Vector3f(7.5F, 0.0F, 15.0F),  EmbreeSingleton::GetInstance().materials["copper"].get(),  nullptr },
        /* object 3 */
        { "/home/Charlie/CGD-CTD/obj/stanford-bunny.obj", 7.5F,    Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 0.0F) * 7.5F),    Eigen::Vector3f(0.0F, -2.25F, 5.0F), EmbreeSingleton::GetInstance().materials["silver"].get(),  nullptr },
        /* object 4 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj",          1000.0F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F)   * 1000.0F), Eigen::Vector3f(0.0F, -2.0F, 10.0F), EmbreeSingleton::GetInstance().materials["white_s"].get(), nullptr },
        /* object 5 */         
        { "/home/Charlie/CGD-CTD/obj/plane.obj",          1000.0F, Eigen::Matrix3f(MakeRotation(90.0F, 0.0F, 0.0F)  * 1000.0F), Eigen::Vector3f(0.0F, 0.0F, 25.0F),  EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
    }
};
}
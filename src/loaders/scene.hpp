#pragma once

#include "lights/light.hpp"
#include "loaders/object.hpp"
#include "loaders/transform.hpp"
#include "camera/camera.hpp"

namespace CT
{
struct Scene
{
    Camera camera;
    Lights lights;
    std::vector<Object> objects;
};

inline Scene double_dragon
{
    /* camera */
    {
        Eigen::Vector3f(0.0F, 0.0F, 0.0F),   // Camera position
        Eigen::Vector3f(0.0F, 0.0F, 1.0F),   // Camera look direction
        Eigen::Vector3f(0.0F, 1.0F, 0.0F),   // Camera up direction
        1.0F
    },

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
            //{ /* area cuboid properties */ }
        },
        /* area_sphere */
        {
            /* area light sphere 1 */
            //{ /* area sphere properties */ }
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

inline Scene triple_statue
{
    /* camera */
    {
        Eigen::Vector3f(0.0F, 0.0F, 0.0F),   // Camera position
        Eigen::Vector3f(0.0F, 0.0F, 1.0F),   // Camera look direction
        Eigen::Vector3f(0.0F, 1.0F, 0.0F),   // Camera up direction
        1.0F
    },
    /* lights */
    {
        /* directional */
        {
            /* directional light 1 */
            { .colour = WHITE  * 0.4F,  .direction = Eigen::Vector3f{ 1.0F, 0.10F, 0.0F } }
            /* directional light 2 */
            //{ .colour = WHITE  * 0.4F,  .direction = Eigen::Vector3f{ -1.0F, 0.1F, 0.0F } }
        },
        /* point */
        {
            /* point light 1 */
            { .colour = WHITE * 100.0F, .position  = Eigen::Vector3f{ 15.0F, 0.0F, 10.0F } }
            /* point light 2 */
            //{ /* point light properties */ }
        },
        /* area_cuboid */
        {
            /* area light cuboid 1 */
            //{ /* area cuboid properties */ }
        },
        /* area_sphere */
        {
            /* area light sphere 1 */
            //{ /* area sphere properties */ }
        }
    },
    /* objects */
    {
    // STATUES
        /* MAX PLANCK */
        { "/home/Charlie/CGD-CTD/obj/max-planck.obj", 0.005F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 0.005F), Eigen::Vector3f(2.3F, -1.125F, 9.5F), EmbreeSingleton::GetInstance().materials["white_s"].get(), nullptr },
        /* LUCY */
        { "/home/Charlie/CGD-CTD/obj/lucy.obj", 0.002F, Eigen::Matrix3f(MakeRotation(-90.0F, 0.0F, 0.0F) * 0.002F), Eigen::Vector3f(-1.45F, -0.3F, 10.0F), EmbreeSingleton::GetInstance().materials["white_s"].get(), nullptr },
        /* NEFERTITI */
        { "/home/Charlie/CGD-CTD/obj/nefertiti.obj", 0.005F, Eigen::Matrix3f(MakeRotation(90.0F, 180.0F, 0.0F) * 0.005F), Eigen::Vector3f(-2.5F, -1.0F, 10.0F), EmbreeSingleton::GetInstance().materials["white_s"].get(), nullptr },

    // CUBES
        /* CUBE LEFT */
        { "/home/Charlie/CGD-CTD/obj/cube.obj", 0.5F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 0.5F), Eigen::Vector3f(2.5F, -2.5F, 10.0F), EmbreeSingleton::GetInstance().materials["black_d"].get(), nullptr },
        /* CUBE MIDDLE */
        { "/home/Charlie/CGD-CTD/obj/cube.obj", 0.5F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 0.5F), Eigen::Vector3f(0.0F, -2.0F, 10.0F), EmbreeSingleton::GetInstance().materials["black_d"].get(), nullptr },
        /* CUBE RIGHT */
        { "/home/Charlie/CGD-CTD/obj/cube.obj", 0.5F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 0.5F), Eigen::Vector3f(-2.5F, -2.75F, 10.0F), EmbreeSingleton::GetInstance().materials["black_d"].get(), nullptr },
    
    // PLANES
        /* GROUND */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 10.0F), Eigen::Vector3f(0.0F, -2.5F, 10.0F), EmbreeSingleton::GetInstance().materials["white_s"].get(), nullptr },
        /* CEILING */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 180.0F) * 10.0F), Eigen::Vector3f(0.0F, 15.5F, 10.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* LEFT WALL_1 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 90.0F) * 10.0F), Eigen::Vector3f(10.0F, 7.5F, 25.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* LEFT WALL_2 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 90.0F) * 10.0F), Eigen::Vector3f(10.0F, 7.5F, -5.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* LEFT WALL_3 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 90.0F) * 10.0F), Eigen::Vector3f(10.0F, 12.5F, 10.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* RIGHT WALL */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 90.0F) * 10.0F), Eigen::Vector3f(-10.0F, 7.5F, 10.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* FRONT WALL*/
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(90.0F, 90.0F, 0.0F) * 10.0F), Eigen::Vector3f(0.0F, 7.5F, 20.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* BACK WALL */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(90.0F, 0.0F, 0.0F) * 10.0F), Eigen::Vector3f(0.0F, 7.5F, 0.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr }
    }
};

inline Scene cornell_box
{
    /* camera */
    {
        Eigen::Vector3f(0.0F, 0.0F, -2.5F),   // Camera position
        Eigen::Vector3f(0.0F, 0.0F, 1.0F),   // Camera look direction
        Eigen::Vector3f(0.0F, 1.0F, 0.0F),   // Camera up direction
        1.0F
    },
    /* lights */
    {        
        /* directional */
        {
            /* directional light 1 */
            //{ .colour = WHITE  * 0.99F,  .direction = Eigen::Vector3f{ 0.0, 0.0F, -1.0F } }
        },
        /* point */
        {
            /* point light 1 */
            //{ .colour = WHITE * 25.0F, .position  = Eigen::Vector3f{ 0.0F, 2.0F, 5.0F } },
            /* point light 2 */
            //{ /* point light properties */ }
        },
        /* area_cuboid */
        {
            /* area light cuboid 1 */
            { WHITE * 10.0F, Eigen::Vector3f(0.0F, 2.4F, 5.0F), Eigen::Vector3f(0.0F, 1.0F, 0.0F), 1.0F, 1.0F },
            { WHITE * 10.0F, Eigen::Vector3f(0.0F, 2.4F, 5.0F), Eigen::Vector3f(0.0F, -1.0F, 0.0F), 1.0F, 1.0F }
        },
        /* area_sphere */
        {
            /* area light sphere 1 */
            { /* area sphere properties */ }
        }
    },
    /* objects */
    {    
    // TEAPOTS
        /* TEAPOT 1 */
        { "/home/Charlie/CGD-CTD/obj/teapot.obj", 0.15F, Eigen::Matrix3f(MakeRotation(0.0F, 60.0F, 0.0F) * 0.15F), Eigen::Vector3f(1.25F, 0.75F, 5.0F), EmbreeSingleton::GetInstance().materials["white_s"].get(), nullptr },
        { "/home/Charlie/CGD-CTD/obj/teapot.obj", 0.15F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 0.15F), Eigen::Vector3f(0.45F, 0.75F, 5.25F), EmbreeSingleton::GetInstance().materials["copper"].get(), nullptr },
        { "/home/Charlie/CGD-CTD/obj/teapot.obj", 0.15F, Eigen::Matrix3f(MakeRotation(0.0F, 50.0F, 0.0F) * 0.15F), Eigen::Vector3f(-0.75F, -1.0F, 4.0F), EmbreeSingleton::GetInstance().materials["jade"].get(), nullptr },

    // CUBES
        // /* CUBE LEFT_1 */
        /* CUBE LEFT_3 */
        { "/home/Charlie/CGD-CTD/obj/cube.obj", 0.75F, Eigen::Matrix3f(MakeRotation(0.0F, 20.0F, 0.0F) * 0.75F), Eigen::Vector3f(0.95F, 0.0F, 5.5F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },

        /* CUBE RIGHT_1 */
        { "/home/Charlie/CGD-CTD/obj/cube.obj", 0.75F, Eigen::Matrix3f(MakeRotation(0.0F, -20.0F, 0.0F) * 0.75F), Eigen::Vector3f(-0.75F, -1.75F, 4.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },

    // PLANES
        /* "LIGHT" */
        //{ "/home/Charlie/CGD-CTD/obj/plane.obj", 0.5F, Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 180.0F)  * 0.5F), Eigen::Vector3f(0.0F, 2.495F, 5.0F), EmbreeSingleton::GetInstance().materials["light"].get(), nullptr },
        
        /* GROUND_1 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(180.0F, 0.0F, 0.0F)   * 2.5F), Eigen::Vector3f(0.0F, -2.5F, 5.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* CEILING_1 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(180.0F, 0.0F, 180.0F) * 2.5F), Eigen::Vector3f(0.0F, 2.5F, 5.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* LEFT WALL_1 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 90.0F)  * 2.5F), Eigen::Vector3f(2.5F, 0.0F, 5.0F), EmbreeSingleton::GetInstance().materials["red_d"].get(), nullptr },
        /* RIGHT WALL_1 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 90.0F)    * 2.5F), Eigen::Vector3f(-2.5F, 0.0F, 5.0F), EmbreeSingleton::GetInstance().materials["green_d"].get(), nullptr },

        /* GROUND_2 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(180.0F, 0.0F, 0.0F)   * 2.5F), Eigen::Vector3f(0.0F, -2.5F, 0.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* CEILING_2 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(180.0F, 0.0F, 180.0F) * 2.5F), Eigen::Vector3f(0.0F, 2.5F, 0.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* LEFT WALL_2 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 90.0F)  * 2.5F), Eigen::Vector3f(2.5F, 0.0F, 0.0F), EmbreeSingleton::GetInstance().materials["red_d"].get(), nullptr },
        /* RIGHT WALL_2 */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 90.0F)    * 2.5F), Eigen::Vector3f(-2.5F, 0.0F, 0.0F), EmbreeSingleton::GetInstance().materials["green_d"].get(), nullptr },

        ///* GROUND_2 */
        ////{ "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(45.0F, 0.0F, 0.0F)    * 2.5F), Eigen::Vector3f(0.0F, -2.5F, 0.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        ///* CEILING_2 */
        //{ "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F)  * 2.5F), Eigen::Vector3f(0.0F, 2.5F, 0.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        ///* LEFT WALL_2 */
        //{ "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 90.0F) * 2.5F), Eigen::Vector3f(2.5F, 0.0F, 0.0F), EmbreeSingleton::GetInstance().materials["red_d"].get(), nullptr },
        ///* RIGHT WALL_2 */
        //{ "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 90.0F)   * 2.5F), Eigen::Vector3f(-2.5F, 0.0F, 0.0F), EmbreeSingleton::GetInstance().materials["green_d"].get(), nullptr },

        /* FRONT WALL*/
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(90.0F, -90.0F, 0.0F)  * 2.5F), Eigen::Vector3f(0.0F, 0.0F, 7.5F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* BACK WALL*/
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 2.5F, Eigen::Matrix3f(MakeRotation(-90.0F, -90.0F, 0.0F)  * 2.5F), Eigen::Vector3f(0.0F, 0.0F, -2.5F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr }
    }
};

inline Scene triple_statue_area_light
{
    /* camera */
    {
        Eigen::Vector3f(0.0F, 0.0F, 0.0F),   // Camera position
        Eigen::Vector3f(0.0F, 0.0F, 1.0F),   // Camera look direction
        Eigen::Vector3f(0.0F, 1.0F, 0.0F),   // Camera up direction
        1.0F
    },
    /* lights */
    {
        /* directional */
        {
            /* directional light 1 */
            //{ .colour = WHITE  * 0.4F,  .direction = Eigen::Vector3f{ 1.0F, 0.10F, 0.0F } }
            /* directional light 2 */
            //{ .colour = WHITE  * 0.4F,  .direction = Eigen::Vector3f{ -1.0F, 0.1F, 0.0F } }
        },
        /* point */
        {
            /* point light 1 */
            //{ .colour = WHITE * 100.0F, .position  = Eigen::Vector3f{ 15.0F, 0.0F, 10.0F } }
            /* point light 2 */
            //{ /* point light properties */ }
        },
        /* area_cuboid */
        {
            /* area light cuboid 1 */
            { WHITE * 25.0F, Eigen::Vector3f(0.0F, 5.0F, 10.0F), Eigen::Vector3f(0.0F, -1.0F, 0.0F), 1.0F, 1.0F }
        },
        /* area_sphere */
        {
            /* area light sphere 1 */
            //{ /* area sphere properties */ }
        }
    },
    /* objects */
    {
    // STATUES
        /* MAX PLANCK */
        { "/home/Charlie/CGD-CTD/obj/max-planck.obj", 0.005F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 0.005F), Eigen::Vector3f(2.3F, -1.125F, 9.5F), EmbreeSingleton::GetInstance().materials["white_s"].get(), nullptr },
        /* LUCY */
        { "/home/Charlie/CGD-CTD/obj/lucy.obj", 0.002F, Eigen::Matrix3f(MakeRotation(-90.0F, 0.0F, 0.0F) * 0.002F), Eigen::Vector3f(-1.45F, -0.3F, 10.0F), EmbreeSingleton::GetInstance().materials["white_s"].get(), nullptr },
        /* NEFERTITI */
        { "/home/Charlie/CGD-CTD/obj/nefertiti.obj", 0.005F, Eigen::Matrix3f(MakeRotation(90.0F, 180.0F, 0.0F) * 0.005F), Eigen::Vector3f(-2.5F, -1.0F, 10.0F), EmbreeSingleton::GetInstance().materials["white_s"].get(), nullptr },

    // CUBES
        /* CUBE LEFT */
        { "/home/Charlie/CGD-CTD/obj/cube.obj", 0.5F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 0.5F), Eigen::Vector3f(2.5F, -2.5F, 10.0F), EmbreeSingleton::GetInstance().materials["black_d"].get(), nullptr },
        /* CUBE MIDDLE */
        { "/home/Charlie/CGD-CTD/obj/cube.obj", 0.5F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 0.5F), Eigen::Vector3f(0.0F, -2.0F, 10.0F), EmbreeSingleton::GetInstance().materials["black_d"].get(), nullptr },
        /* CUBE RIGHT */
        { "/home/Charlie/CGD-CTD/obj/cube.obj", 0.5F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 0.5F), Eigen::Vector3f(-2.5F, -2.75F, 10.0F), EmbreeSingleton::GetInstance().materials["black_d"].get(), nullptr },
    
    // PLANES
        /* GROUND */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 0.0F) * 10.0F), Eigen::Vector3f(0.0F, -2.5F, 10.0F), EmbreeSingleton::GetInstance().materials["white_s"].get(), nullptr },
        // /* CEILING */
        // { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 180.0F) * 10.0F), Eigen::Vector3f(0.0F, 15.5F, 10.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        // /* LEFT WALL_1 */
        // { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(0.0F, 180.0F, 90.0F) * 10.0F), Eigen::Vector3f(10.0F, 7.5F, 10.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        // /* RIGHT WALL */
        // { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(0.0F, 0.0F, 90.0F) * 10.0F), Eigen::Vector3f(-10.0F, 7.5F, 10.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        // /* FRONT WALL*/
        // { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(90.0F, 90.0F, 0.0F) * 10.0F), Eigen::Vector3f(0.0F, 7.5F, 20.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr },
        /* BACK WALL */
        { "/home/Charlie/CGD-CTD/obj/plane.obj", 10.0F, Eigen::Matrix3f(MakeRotation(90.0F, 0.0F, 0.0F) * 10.0F), Eigen::Vector3f(0.0F, 7.5F, 0.0F), EmbreeSingleton::GetInstance().materials["white_d"].get(), nullptr }
    }
};

}
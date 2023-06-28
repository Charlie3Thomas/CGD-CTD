#pragma once

#include <Eigen/Core>

#include <array>

using namespace Eigen;

namespace CT
{
struct Vertex
{
    Vector3f pos;
    Vector3f vertex_normal;
    //Vector3f face_normal;
};

struct Triangle
{
    Triangle() = default;
    Triangle(Vertex A, Vertex B, Vertex C) : v{A, B, C} 
    {
        // Vector3f U = v[1].pos - v[0].pos;
        // Vector3f V = v[2].pos - v[0].pos;

        // // Calculate face normal
        // Vector3f fn = Vector3f( (U.y() * V.z()) - (U.z() * V.y()),
        //                         (U.z() * V.x()) - (U.x() * V.z()),
        //                         (U.x() * V.y()) - (U.y() * V.x()));
        // fn.normalize();

        // for (int i = 0; i < 3; i++)
        //     v[i].face_normal = fn;
        
        // p = cross(B-A, C-A)
        Vector3f p = (B.pos - A.pos).cross(C.pos - A.pos);

        A.vertex_normal += p;
        B.vertex_normal += p;
        C.vertex_normal += p;

        for (int i = 0; i < 3; i++)
            v[i].vertex_normal.normalize();
    }

    std::array<Vertex, 3> v;
};

struct Face
{
    std::array<uint32_t, 3> v;
};

struct UVTextureCoords
{
    std::array<Vector2f, 3> coords;
};

}
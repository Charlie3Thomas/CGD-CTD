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
    Triangle(Vertex A, Vertex B, Vertex C) : v{A, B, C} {}

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
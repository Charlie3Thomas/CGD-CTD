#pragma once

#include <Eigen/Core>

#include <array>

using namespace Eigen;

namespace CT
{
struct Vertex
{
    Vector3f pos;
};

struct Triangle
{
    Triangle() = default;
    Triangle(Vertex v1, Vertex v2, Vertex v3) : v{v1, v2, v3} {}
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
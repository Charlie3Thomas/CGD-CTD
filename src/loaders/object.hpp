#pragma once

#include "materials/mat.hpp"
#include "loaders/prims.hpp"

#include <Eigen/Core>

#include <filesystem>
#include <unordered_map>
#include <array>
#include <vector>

namespace CT
{
struct Object
{
    // Path to .obj file
    std::filesystem::path p_file;

    // Object scale
    float scale;

    // Object transformation
    Eigen::Matrix3f transformation;

    // Object translation
    Eigen::Vector3f translation;

    // Object material
    const Mat* material;

    // Object texture
    const Texture* texture;

    // Texture coordinates
    std::unordered_map<unsigned int, UVTextureCoords> tex_coords;

    // Triangles
    std::vector<Triangle> triangles; 
};    
}
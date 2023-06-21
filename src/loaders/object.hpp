#pragma once

#include "materials/material.hpp"

#include <Eigen/Core>

#include <filesystem>

using namespace Eigen;

namespace CT
{
struct Object
{
    // Path to .obj file
    std::filesystem::path p_file;

    // Object scale
    float scale;

    // Object transformation
    Matrix3f transformation;

    // Object translation
    Vector3f translation;

    // Object material
    const Material* material;

    // Object texture
    const Texture* texture;
};    
}
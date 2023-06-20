#pragma once

#include "materials/material.hpp"

#include <Eigen/Core>

#include <filesystem>

using namespace Eigen;

namespace CT
{
struct Object
{
    Object(std::filesystem::path p_file, float scale, Matrix3f& transformation, Vector3f& translation, Material& material/*, Texture& texture*/)
        : p_file(p_file), scale(scale), transformation(transformation), translation(translation), material(material)/*, texture(texture)*/ {}

    // Path to .obj file
    std::filesystem::path p_file;

    // Object scale
    float scale;

    // Object transformation
    Matrix3f transformation;

    // Object translation
    Vector3f translation;

    // Object material
    Material material;

    // Object texture
    //Texture texture;
};    
}
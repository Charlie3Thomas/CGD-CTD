#pragma once

#include "materials/mat.hpp"

namespace CT
{
struct PhongMat : public Mat
{
public:
    PhongMat(const RGB& diffuse_colour, const RGB& specular_colour, float shininess, float albedo)
        : diffuse_colour(diffuse_colour), specular_colour(specular_colour), shininess(shininess), albedo(albedo) {}

    RGB Evaluate(const Vector3f& incident, const Vector3f& normal) const override;

    float PDF(const Vector3f& incident, const Vector3f& normal) const override;

    Vector3f SampleBDRF(const Vector3f& incident, const Vector3f& normal) const override;

private:
    RGB diffuse_colour;
    RGB specular_colour;
    float shininess;
    float albedo;
};

}
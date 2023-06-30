#include "materials/phongmat.hpp"
#include "utils/utils.hpp"

namespace CT
{

RGB PhongMat::Evaluate(const Vector3f& incident, const Vector3f& normal) const
{
    return diffuse_colour;
}

float PhongMat::PDF(const Vector3f& incident, const Vector3f& normal) const
{
    Vector3f reflected_dir = incident - 2.0F * incident.dot(normal) * normal;
    float cos_theta = reflected_dir.dot(normal);
    float pdf_value = (shininess + 1.0F) * std::pow(cos_theta, shininess) / (2.0F * M_PIf32);
    return pdf_value;
}

Vector3f PhongMat::SampleBDRF(const Vector3f& incident, const Vector3f& normal) const
{
    // Material properties
    
    Vector3f reflected_dir = incident - 2.0F * incident.dot(normal) * normal;
    Vector3f tangent = reflected_dir.cross(normal);
    Vector3f bitangent = reflected_dir.cross(tangent);

    float u = RandomRange(0.0F, 1.0F);
    float v = RandomRange(0.0F, 1.0F);

    float phi = 2.0F * M_PIf32 * u;
    float exponent = 1.0F / (shininess + 1.0F);
    float cos_theta = std::pow(v, exponent);
    float sin_theta = std::sqrt(1.0F - cos_theta * cos_theta);

    float x = sin_theta * std::cos(phi);
    float y = sin_theta * std::sin(phi);
    float z = cos_theta;

    return x * tangent + y * bitangent + z * reflected_dir;
}

}
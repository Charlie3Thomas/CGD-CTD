#include "pointlight.hpp"

CT::PointLight::PointLight(const Eigen::Vector3f& position, const float intensity)
    : position(position), intensity(intensity) {}

float CT::PointLight::SampleLight() const
{
    return intensity;
}

float CT::PointLight::EvaluateLight(const Eigen::Vector3f& point) const
{
    return intensity;
}

float CT::PointLight::Pdf(const Eigen::Vector3f& point) const
{
    return 1.0F;
}
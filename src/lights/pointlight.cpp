#include "pointlight.hpp"

CT::PointLight::PointLight(Eigen::Vector3f& position, const float intensity)
    : position(std::move(position)), intensity(intensity) {}

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
#pragma once

#include "utils/rgb.hpp"

#include <Eigen/Dense>

#include <random>

using namespace Eigen;

namespace CT
{
struct Mat
{
public:
    virtual ~Mat() = default;

    virtual RGB Evaluate(const Vector3f& incident, const Vector3f& normal) const = 0;

    virtual float PDF(const Vector3f& incident, const Vector3f& normal) const = 0;

    virtual Vector3f SampleBDRF(const Vector3f& incident, const Vector3f& normal) const = 0;

    virtual Vector3f SampleHemishphere(const Vector3f& normal);
};
}
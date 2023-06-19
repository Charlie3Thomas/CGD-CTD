#pragma once

#include <cstddef>

#include <Eigen/Dense>

namespace CT
{
class Light
{
public:
    virtual ~Light() {};

    virtual float SampleLight() const = 0;

    virtual float EvaluateLight(const Eigen::Vector3f& point) const = 0;

    virtual float Pdf(const Eigen::Vector3f& point) const = 0;

};
}
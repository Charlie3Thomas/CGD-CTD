#pragma once

#include "utils/rgb.hpp"

#include <Eigen/Dense>

using namespace Eigen;

namespace CT
{
struct Mat
{
    RGB ka;
    RGB kd;
    RGB ks;
    float shininess;
};
}
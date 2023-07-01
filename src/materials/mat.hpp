#pragma once

#include "utils/rgb.hpp"

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
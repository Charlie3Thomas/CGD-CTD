#pragma once

#include "utils/rgb.hpp"

namespace CT
{
struct Mat
{
    RGB kd;
    RGB ks;
    float shininess;
    bool specular;
};
}
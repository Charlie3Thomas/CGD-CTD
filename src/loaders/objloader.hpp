#pragma once

#include <cstdint>
#include <vector>

#include <Eigen/Core>
#include <embree3/rtcore.h>

namespace CT
{
unsigned int LoadObj(const char* p_file, std::vector<RTCBuildPrimitive>& prims, const Eigen::Matrix3f& transformation, const Eigen::Vector3f& translation);
}
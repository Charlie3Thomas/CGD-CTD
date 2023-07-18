#pragma once

#include "loaders/object.hpp"

#include <cstdint>
#include <vector>

#include <Eigen/Core>
#include <embree3/rtcore.h>

namespace CT
{
class ObjectLoader
{
public:
    void LoadObjects(std::vector<Object>& objects);
    std::vector<RTCBuildPrimitive>& GetPrims();

private:
    // BVH prims data
    std::vector<RTCBuildPrimitive> prims;
};

}
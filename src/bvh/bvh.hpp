#pragma once

#include <embree3/rtcore.h>
#include <vector>
#include <iostream>
#include <cassert>

namespace CT
{
float Area(const RTCBounds& b);
//RTCBounds merge(const RTCBounds& a, const RTCBounds& b);

void ErrCallback(void* user_ptr, RTCError code, const char* str);
void BuildBVH(RTCBuildQuality quality, std::vector<RTCBuildPrimitive>& prims_i, char* cfg, size_t extra_space = 0);
}
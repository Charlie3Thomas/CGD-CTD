#pragma once

#include <embree3/rtcore.h>
#include <vector>
#include <iostream>
#include <cassert>

#include "embree/embreedevice.hpp"

namespace CT
{
float Area(const RTCBounds& b);
RTCBounds merge(const RTCBounds& a, const RTCBounds& b);

void CreateNode();
void SetNodeChildren();
void SetNodeBounds();
void CreateLeaf();
void SplitPrimitive();
void BuildProgress();

void ErrCallback(void* user_ptr, RTCError code, const char* str);
void BuildBVH(RTCBuildQuality quality, std::vector<RTCBuildPrimitive>& prims_i, char* cfg, size_t extra_space = 0);
}
#pragma once

#include <embree3/rtcore.h>
#include <vector>
#include <iostream>
#include <cassert>

#include "embree/embreedevice.hpp"

void ErrCallback(void* user_ptr, RTCError code, const char* str);
void BuildBVH(RTCBuildQuality quality, std::vector<RTCBuildPrimitive>& prims_i, char* cfg, size_t extra_space = 0);
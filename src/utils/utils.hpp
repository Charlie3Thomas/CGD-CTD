#pragma once

#include <cstdint>

namespace CT
{
uint64_t GetGUID();

// template<typename T>
// T RandomRange(T min, T max);

float RandomRange(float min, float max);

void ToUnitDisk(double seedx, double seedy, double *x, double *y);
}
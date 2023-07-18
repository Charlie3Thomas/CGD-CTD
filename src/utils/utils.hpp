#pragma once

#include <cstdint>

namespace CT
{
uint64_t GetGUID();

float RandomRange(float min, float max);

float RandomValueNormalDistrubution();

void ToUnitDisk(double seedx, double seedy, double *x, double *y);
}
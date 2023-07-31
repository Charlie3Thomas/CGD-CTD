#pragma once

#include <cstdint>
#include <cstddef>

namespace CT
{
uint64_t GetGUID();

float RandomRange(float min, float max);

float RandomValueNormalDistrubution();

void ToUnitDisk(double seedx, double seedy, double *x, double *y);

float* LoadEXRFromFile(const char* filenamem, int& width, int& height);

float L1Difference(const float* a, const float* b, size_t height, size_t width);

float L2Difference(const float* a, const float* b, size_t height, size_t width);
}
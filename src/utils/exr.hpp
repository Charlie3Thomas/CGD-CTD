#pragma once
#include <cstddef>

namespace CT
{
void WriteToEXR(const float* rgb, size_t width, size_t height, const char* outfilename);
}
#pragma once

#include <FreeImage.h>
#include <iostream>
#include <cstdint>
#include <vector>

namespace CT
{
struct Texture
{
    std::vector<BYTE> buffer;
    unsigned int width;
    unsigned int height;
};

Texture LoadTexture();

}
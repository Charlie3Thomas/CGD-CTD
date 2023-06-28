#pragma once

#include <FreeImage.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include <filesystem>

namespace CT
{
struct Texture
{
    Texture(std::filesystem::path fp);
    std::vector<BYTE> buffer;
    unsigned int width;
    unsigned int height;
};

Texture LoadTexture(std::filesystem::path fp);
}
#pragma once
#include <cstddef>
#include <vector>

/// @brief Subdivisions of the image containing pixel data
class Canvas
{
public:
    Canvas(size_t width, size_t height);
    ~Canvas() = default;

    void WritePixelData(float r, float g, float b, int pixel_index);

    std::vector<float> GetPixelData() const;

private:
    std::vector<float> _rgb;
};
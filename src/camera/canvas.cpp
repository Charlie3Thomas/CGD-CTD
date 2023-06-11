#include "canvas.hpp"

Canvas::Canvas(size_t width, size_t height)
{
    // Allocate memory for pixel data
    _rgb = std::vector<float>(width * height * 3);
}

void Canvas::WritePixelData(float r, float g, float b, int pixel_index)
{
    _rgb[3 * pixel_index + 0] = r;
    _rgb[3 * pixel_index + 1] = g;
    _rgb[3 * pixel_index + 2] = b;
}

std::vector<float> Canvas::GetPixelData() const
{
    return _rgb;
}
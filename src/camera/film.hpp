#pragma once

#include "canvas.hpp"

/// @brief Full image containing canvas tiles
class Film
{
public: 
    Film(size_t width, size_t height, size_t divisions);
    ~Film() = default;

    void WritePixelDataToCanvas(float r, float g, float b, int canvas_index, int pixel_index);

    std::vector<float> GetPixelData() const;

private:
    std::vector<Canvas> canvases;
};
#pragma once

#include "canvas.hpp"
#include "rect.hpp"

#include <Eigen/Dense>

namespace CT
{
/// @brief Full image containing canvas tiles
class Film
{
public: 
    Film(const size_t width, const size_t height, const Eigen::Vector2i& canvas_size);

    void DevelopFilm();

    Rect rect;

    std::vector<Canvas> canvases;

    std::vector<float> rgb;
};
}
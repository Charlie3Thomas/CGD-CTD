#pragma once

#include "rect.hpp"

#include <cstddef>
#include <vector>


namespace CT
{
class Film;

/// @brief Subdivisions of the image containing pixel data
class Canvas
{
public:
    Canvas(Film& film);

    Rect rect;

    /// @brief Reference to a pixel in the film
    struct PixelRef
    {
        float& r;
        float& g;
        float& b;
    };

    /// @brief Returns a reference relative to the film pixel at a local singluar index
    /// @param index 
    /// @return 
    PixelRef operator[](size_t index);

    /// @brief Returns a reference to the film pixel at the given canvas coordinates
    /// @param x 
    /// @param y 
    /// @return 
    PixelRef operator()(size_t x, size_t y);

    const Film& GetFilm() const { return _film; }

private:
    Film& _film;
};
}
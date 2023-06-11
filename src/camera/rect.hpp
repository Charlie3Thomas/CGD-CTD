#pragma once
#include <cstddef>
#include <algorithm>

struct Rect
{
    Rect() = default;

    // Use for film
    Rect(size_t width, size_t height) 
        : lrx(width - 1), lry(height - 1) {}

    // Upper left x
    size_t ulx = 0;

    // Upper left y
    size_t uly = 0;

    // Lower right x
    size_t lrx;
    
    // Lower right y
    size_t lry;

    void Intersect(const Rect& other);
    
    size_t GetWidth() const { return  lrx - ulx + 1; }
    size_t GetHeight() const { return lry - uly + 1; }
};

inline void Rect::Intersect(const Rect& other)
{   
    // Upper left point
    ulx = std::max(ulx, other.ulx);
    uly = std::max(uly, other.uly);

    // Lower right point
    lrx = std::min(lrx, other.lrx);
    lry = std::min(lry, other.lry);

    // Ensure width and height are non-negative
    lrx = std::max(lrx, ulx);
    lry = std::max(lry, uly);
    ulx = std::min(lrx, ulx);
    uly = std::min(lry, uly);
}


#include "canvas.hpp"
#include "film.hpp"

namespace CT
{
Canvas::Canvas(Film& film) : _film(film) { }

Canvas::PixelRef Canvas::operator[](size_t index)
{
    const size_t x = index % rect.GetWidth();
    const size_t y = index / rect.GetWidth();

    return (*this)(x, y);
}

Canvas::PixelRef Canvas::operator()(size_t x, size_t y)
{
    const size_t film_width = _film.rect.GetWidth();
    const size_t film_x     = x + rect.ulx;
    const size_t film_y     = y + rect.uly;
    const size_t film_index = film_y * film_width + film_x;

    return { _film.rgb[film_index * 3 + 0],
             _film.rgb[film_index * 3 + 1],
             _film.rgb[film_index * 3 + 2] };
}
}
#include "film.hpp"


namespace CT
{
Film::Film(const size_t width, const size_t height, const Eigen::Vector2i& canvas_size) : rect(width, height), rgb(width * height * 3)
{
    const size_t xc = rect.GetWidth()  / canvas_size.x() + static_cast<size_t>(rect.GetWidth()  % canvas_size.x() != 0);
    const size_t yc = rect.GetWidth()  / canvas_size.y() + static_cast<size_t>(rect.GetHeight() % canvas_size.y() != 0);

    for (size_t h = 0; h < yc; h++)
    {
        for (size_t w = 0; w < xc; w++)
        {
            // Create canvas
            Canvas c(*this);

            // Set canvas rect
            c.rect.ulx = w          * canvas_size.x();
            c.rect.uly = h          * canvas_size.y();
            c.rect.lrx = c.rect.ulx + canvas_size.x() - 1;
            c.rect.lry = c.rect.uly + canvas_size.y() - 1;

            // Ensure canvas fits within film bounds
            c.rect.Intersect(rect);

            // Add canvas to film
            canvases.emplace_back(std::move(c));
        }
    }

    std::fill(rgb.begin(), rgb.end(), 1.0F);
}

void Film::DevelopFilm()
{
    // TODO : construct film from canvases and write data to _rgb
}
}
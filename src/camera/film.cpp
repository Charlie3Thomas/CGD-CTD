#include "film.hpp"

#include <cassert>

Film::Film(size_t width, size_t height, size_t divisions) // TODO: Add divisions to config
{
    // This may not be the best way of doing this
    // Canvases don't know what their pixel span is, only absolute size
    // Might be worth replacing divisions with minimum canvas size
    // 30x30 / 40x40 makes sense as a minimum canvas size as they're common factors of most standard resolutions
    for (size_t i = 0; i < divisions; i++)
    {
        Canvas c (width / divisions, height / divisions);
        canvases.emplace_back(c);
    }
}

void Film::WritePixelDataToCanvas(float r, float g, float b, int canvas_index, int pixel_index)
{
    // Make sure canvas index is valid
    assert(canvas_index < static_cast<int>(canvases.size()));

    canvases[canvas_index].WritePixelData(r, g, b, pixel_index);
}

 std::vector<float> Film::GetPixelData() const
 {
    std::vector<float> ret;

    for (const auto& canvas : canvases)
    {
        std::vector<float> canvas_data = canvas.GetPixelData();
        ret.insert(ret.end(), canvas_data.begin(), canvas_data.end());
    }

    return ret;
 }
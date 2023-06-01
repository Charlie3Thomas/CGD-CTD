#pragma once

#include <filesystem>
#include <iostream>

#include "rgb.hpp"

namespace CT
{
/// @brief Write the header of a PPM file to the given stream.
    /// @param os 
    /// @param width 
    /// @param height 
    /// @return 
std::ostream& PPMWriteHeader(std::ostream& os, std::size_t width, std::size_t height);

/// @brief Write a pixel to the given stream.
    /// @param os 
    /// @param rgb A type containing the red, green, and blue values of the pixel.
    /// @return 
std::ostream& PPMWritePixel(std::ostream& os, CT::RGB rgb);
}
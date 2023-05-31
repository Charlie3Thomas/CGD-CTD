#pragma once

#include "common.hpp"

#include <filesystem>
#include <iostream>

/**
 * @brief Converts a PPM to PNG
 * The way it does this at the moment is hacky and required Linux system calls - a much nicer way of doing this would be to use libpng.
 * The original PPM file is deleted. A runtime exception will be thrown if any of the system calls fail  
 * @param ppm The original PPM file
 * @return auto The name of the returned PNG file
 */
std::filesystem::path ppm_to_png(const std::filesystem::path& ppm_file);

/**
 * @brief Writes the common PPM header
 */
std::ostream& ppm_write_header(std::ostream& os, std::size_t horizontal_pixels, std::size_t vertical_pixels, std::size_t bits = std::numeric_limits<std::uint8_t>::max());

/**
 * @brief Writes a PPM pixel
 * Must have already written the header
 */
template <typename T>
std::ostream& ppm_write_pixel(std::ostream& os, const rgb_t<T>& pixel)
{
    return os << pixel << '\n';
}
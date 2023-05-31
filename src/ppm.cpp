#include "ppm.hpp"
#include <filesystem>

std::filesystem::path ppm_to_png(const std::filesystem::path& ppm_file)
{
    // yes this is bad, no i don't care
    auto png_file = std::filesystem::path(ppm_file).replace_extension(".png");
    if (std::system(("convert " + std::string(ppm_file) + ' ' + std::string(png_file)).c_str()) != 0)
        throw std::runtime_error("Could not convert " + std::string(ppm_file) + " to " + std::string(png_file));
    
    if (!std::filesystem::remove(ppm_file))
        throw std::runtime_error("Could not delete " + std::string(ppm_file));

    return png_file;
}

std::ostream& ppm_write_header(std::ostream& os, std::size_t horizontal_pixels, std::size_t vertical_pixels, std::size_t bits)
{
    return os << "P3\n" << horizontal_pixels << " " << vertical_pixels << '\n' << bits << '\n';
}
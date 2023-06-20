#include "exr.hpp"

#include "utils/timer.hpp"

#include <tinyexr.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <array>
#include <cassert>


namespace CT
{

/// @brief Saves an EXR file from a float array
/// @param rgb An array of floats representing the RGB values of the image
/// @param width Width of the image
/// @param height Height of the image
/// @param outfilename File path and name of the output file
/// @return 
void WriteToEXR(const float* rgb, size_t width, size_t height, const char* outfilename) //TODO: replace const chat* with std::filesystem::path
{
    Timer t = Timer("WriteToEXR");

    EXRHeader header;
    InitEXRHeader(&header);

    EXRImage image;
    InitEXRImage(&image);

    image.num_channels = 3;

    std::array<std::vector<float>, 3> images;
    images[0].resize(width * height);
    images[1].resize(width * height);
    images[2].resize(width * height);

    // Split RGBRGBRGB... into R, G and B layer
    for (size_t i = 0; i < width * height; i++)
    {
        images[0][i] = rgb[3*i+0]; // R
        images[1][i] = rgb[3*i+1]; // G
        images[2][i] = rgb[3*i+2]; // B
    }

    std::array<float*, 3> img_array;
    img_array[0] = images[2].data(); // B
    img_array[1] = images[1].data(); // G
    img_array[2] = images[0].data(); // R

    image.images = reinterpret_cast<unsigned char**>(img_array.data());
    image.width  = static_cast<int>(width);
    image.height = static_cast<int>(height);

    constexpr size_t num_channels = 3;
    header.num_channels = static_cast<int>(num_channels);
    std::array<EXRChannelInfo, num_channels> channels;
    header.channels = channels.data();

    // Must be (A)BGR order, since most EXR viewers expect this channel order.
    strncpy(&header.channels[0].name[0], "\0", 255); header.channels[0].name[0] = 'B';
    strncpy(&header.channels[1].name[0], "\0", 255); header.channels[1].name[0] = 'G';
    strncpy(&header.channels[2].name[0], "\0", 255); header.channels[2].name[0] = 'R';

    header.pixel_types           = new int[header.num_channels];
    header.requested_pixel_types = new int[header.num_channels];

    for (int i = 0; i < header.num_channels; i++) 
    {
        // pixel type of input image
        header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;

        // pixel type of output image to be stored in .EXR
        header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; 
    }    

    const char* err = nullptr;
    int ret = SaveEXRImageToFile(&image, &header, outfilename, &err);

    delete[] header.pixel_types;
    delete[] header.requested_pixel_types;

    assert(ret == TINYEXR_SUCCESS);

    std::cout << ret << std::endl;
}
}
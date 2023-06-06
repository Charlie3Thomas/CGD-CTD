#include "exr.hpp"

#include <tinyexr.h>
#include <cstring>
#include <iostream>
#include <vector>

/// @brief Saves an EXR file from a float array
/// @param rgb An array of floats representing the RGB values of the image
/// @param width Width of the image
/// @param height Height of the image
/// @param outfilename File path and name of the output file
/// @return 
bool WriteToEXR(const float* rgb, int width, int height, const char* outfilename)
{
    EXRHeader header;
    InitEXRHeader(&header);

    EXRImage image;
    InitEXRImage(&image);

    image.num_channels = 3;

    std::vector<float> images[3];
    images[0].resize(width * height);
    images[1].resize(width * height);
    images[2].resize(width * height);

    // Split RGBRGBRGB... into R, G and B layer
    for (int i = 0; i < width * height; i++)
    {
        images[0][i] = rgb[3*i+0]; // R
        images[1][i] = rgb[3*i+1]; // G
        images[2][i] = rgb[3*i+2]; // B
    }

    float* image_ptr[3];
    image_ptr[0] = &(images[2].at(0)); // B
    image_ptr[1] = &(images[1].at(0)); // G
    image_ptr[2] = &(images[0].at(0)); // R

    image.images = (unsigned char**)image_ptr;
    image.width = width;
    image.height = height;

    header.num_channels = 3;
    header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
    // Must be (A)BGR order, since most of EXR viewers expect this channel order.
    strncpy(header.channels[0].name, "B", 255); header.channels[0].name[strlen("B")] = '\0';
    strncpy(header.channels[1].name, "G", 255); header.channels[1].name[strlen("G")] = '\0';
    strncpy(header.channels[2].name, "R", 255); header.channels[2].name[strlen("R")] = '\0';

    header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
    header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
    for (int i = 0; i < header.num_channels; i++) 
    {
        // pixel type of input image
        header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;

        // pixel type of output image to be stored in .EXR
        header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; 
    }

    const char* err = nullptr;
    int ret = SaveEXRImageToFile(&image, &header, outfilename, &err);
    if (ret != TINYEXR_SUCCESS)
    {
        std::cout << stderr << "Save EXR err: %s\n" << err << std::endl;
        FreeEXRErrorMessage(err);
        return ret;
    }

    std::cout << "Saved EXR file.\n" << outfilename << std::endl;
    //free(rgb);
    free(header.channels);
    free(header.pixel_types);
    free(header.requested_pixel_types);
}
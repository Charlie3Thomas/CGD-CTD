#include <FreeImage.h>
#include <iostream>
#include <cassert>

#include "embree/embreedevice.hpp"


void FreeImageTest(RTCBuffer &b)
{
    FreeImage_Initialise();

    const char* path = "/home/Charlie/CGD-CTD/textures/epoca.jpg";
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path, 0);
    FIBITMAP* image = FreeImage_Load(format, path);
    if (image == nullptr)
    {
        std::cout << "Failed to load image: " << path << std::endl;
        FreeImage_DeInitialise();
    }

    // Get the width and height of the image
    unsigned int width  = FreeImage_GetWidth(image);
    unsigned int height = FreeImage_GetHeight(image);

    BYTE* image_data = FreeImage_GetBits(image);

    // Create an RTCBuffer to hold the image data
    b = rtcNewSharedBuffer(EmbreeSingleton::GetInstance().device, image_data, static_cast<size_t>(width) * height * 3 * sizeof(unsigned char));
    assert(b != nullptr);
}


int main (int argc, char** argv)
{
    return EXIT_SUCCESS;
}
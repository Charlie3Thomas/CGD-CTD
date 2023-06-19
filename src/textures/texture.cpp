#include "texture.hpp"

namespace CT
{
Texture LoadTexture()
{
    FreeImage_Initialise();

    const char* path = "/home/Charlie/CGD-CTD/textures/water.png";
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

    size_t buffer_size = (static_cast<unsigned long>(width) * static_cast<unsigned long>(height) * sizeof(BYTE)) * 3;

    std::vector<BYTE> buffer(image_data, image_data + buffer_size);

    FreeImage_Unload(image);
    FreeImage_DeInitialise();

    return Texture{ buffer, width, height };
}
}
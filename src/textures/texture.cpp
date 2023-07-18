#include "textures/texture.hpp"

namespace CT
{
Texture::Texture(std::filesystem::path fp)
{
    FreeImage_Initialise();

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fp.c_str(), 0);
    FIBITMAP* image = FreeImage_Load(format, fp.c_str());
    if (image == nullptr)
    {
        std::cout << "Failed to load image: " << fp << std::endl;
        FreeImage_DeInitialise();
    }

    // Get the width and height of the image
    width  = FreeImage_GetWidth(image);
    height = FreeImage_GetHeight(image);


    BYTE* image_data = FreeImage_GetBits(image);

    size_t buffer_size = (static_cast<unsigned long>(width) * static_cast<unsigned long>(height) * sizeof(BYTE)) * 3;

    buffer = std::vector<BYTE>(image_data, image_data + buffer_size);

    FreeImage_Unload(image);
    FreeImage_DeInitialise();
}
}
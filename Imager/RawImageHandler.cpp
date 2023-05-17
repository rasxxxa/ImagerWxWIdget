#define STB_IMAGE_IMPLEMENTATION
#include "../STB/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../STB/stb_image_write.h"
#include "RawImageHandler.h"

[[nodiscard]] Image RawImageHandler::LoadImageFile(const std::string& path)
{
    // Image properties
    int width, height, channels;

    ImageType type = ImageType::PNG;
    std::string copy = path;
    for (unsigned ch = 0; ch < copy.size(); ch++)
    {
        copy[ch] = std::tolower(copy[ch]);
    }

    if (std::equal(copy.end() - 4, copy.end(), ".png"))
    {
        type = ImageType::PNG;
    }
    else if (std::equal(copy.end() - 4, copy.end(), ".jpg"))
    {
        type = ImageType::JPG;
    }
    else
    {
        type = ImageType::BMP;
    }

    // Load the image using stb_image
    unsigned char* image = stbi_load(path.data(), &width, &height, &channels, 0);
    if (!image) 
    {
        std::cout << "Failed to load image, reason: " << stbi_failure_reason() << " \n";
        return Image{};
    }
    Image im;
    im.m_channels = channels;
    im.m_height = height;
    im.m_width = width;
    im.m_size = height * width * channels;
    im.m_imageData = std::vector<char>(image, image + im.m_size);
    im.type = type;
    im.path = path;

    stbi_image_free(image);
    return im;
}

bool RawImageHandler::WriteImage(const Image& image) const
{
    static unsigned serial = 0;
    std::string withExt = image.path;
    withExt.insert(withExt.size() - 4, std::to_string(serial++).data());
    switch (image.type)
    {
    case ImageType::PNG: {
    
        return stbi_write_png(withExt.data(), image.m_width, image.m_height, image.m_channels, image.m_imageData.data(), image.m_width * image.m_channels);
    
    }break;
    case ImageType::JPG: {return true; }break;
    case ImageType::BMP: {return true; }break;
    default:
        break;
    }
    return false;
}

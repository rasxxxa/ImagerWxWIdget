#pragma once
#include <vector>
#include <string>
#include <iostream>

enum class ImageType
{
	PNG,
	JPG,
	BMP,
};

struct Image
{
	std::vector<unsigned char> m_imageData;
	unsigned int m_width;
	unsigned int m_height;
	unsigned long long m_size;
	unsigned short m_channels;
	ImageType type = ImageType::PNG;
	std::string path;
};

class RawImageHandler
{
public:
	RawImageHandler() = default;
	[[nodiscard]]Image LoadImageFile(const std::string& path);
	bool WriteImage(const Image& image) const;
	std::pair<std::vector<unsigned char>, std::vector<unsigned char>> SeparateAlpha(Image& image);
};


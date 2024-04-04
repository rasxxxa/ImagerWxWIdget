#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <optional>

enum class ImageType
{
	PNG,
	JPG,
	BMP,
};

struct RGB
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
};

using MATRIX = std::vector<std::vector<RGB>>;
using IMAGEDATA = std::vector<unsigned char>;
using IMAGEDATACOPY = std::vector<int>;

struct Image
{
	IMAGEDATA m_imageData;
	IMAGEDATACOPY m_imageDataCopy;
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
	bool WriteImage(const Image& image, std::optional<std::string> path = std::nullopt) const;
	std::pair<std::vector<unsigned char>, std::vector<unsigned char>> SeparateAlpha(Image& image);

	static MATRIX CreateMatrixFromImage(const Image& image);
	static IMAGEDATA CreateImageFromMatrix(const MATRIX& matrix);

};


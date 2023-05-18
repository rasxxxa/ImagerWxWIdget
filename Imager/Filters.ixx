#include "RawImageHandler.h"

export module Filters;

export
{
	void GrayFilter(Image& image)
	{
		auto& data = image.m_imageData;

		for (unsigned pix = 0; pix < image.m_imageData.size(); pix += 3)
		{
			unsigned char val = data[pix] * 0.11f + data[pix + 1] * 0.66f + data[pix + 2] * 0.23f;
			data[pix] = val;
			data[pix + 1] = val;
			data[pix + 2] = val;
		}

	}
}
#include "RawImageHandler.h"
#include <thread>
#include <functional>


export module Filters;

export
{
	constexpr size_t MAX_NUMBER_OF_THREADS = 4;

	struct RGB
	{
		unsigned char R;
		unsigned char G;
		unsigned char B;
		unsigned char A;
	};

	void GrayFilter(Image& image, unsigned left, unsigned right, unsigned bitOffset)
	{
		auto& data = image.m_imageData;
		std::cout << &data << std::endl;
		for (unsigned pix = left; pix < right; pix += (4 * bitOffset))
		{
			unsigned char val = data[pix] * 0.11f + data[pix + 1] * 0.66f + data[pix + 2] * 0.23f;
			data[pix] = val;
			data[pix + 1] = val;
			data[pix + 2] = val;
		}
	}

	void ThreadDivisionOfFunction(Image& image, bool useMatrixFormat, size_t bitOffset, const std::function<void(Image&, unsigned, unsigned, unsigned)>& function)
	{
		std::vector<std::jthread> threads;
		threads.reserve(MAX_NUMBER_OF_THREADS);

		if (useMatrixFormat)
		{
			std::vector<std::vector<RGB>> matrix(image.m_height, std::vector<RGB>(image.m_width, RGB()));
			unsigned i = 0, j = 0;
			for (unsigned long pix = 0; pix < image.m_imageData.size(); pix+=4)
			{
				RGB t;
				t.R = image.m_imageData[pix];
				t.G = image.m_imageData[pix + 1];
				t.B = image.m_imageData[pix + 2];
				t.A = image.m_imageData[pix + 3];
				matrix[i][j++] = t;
				if (j == image.m_width)
				{
					i++;
					j = 0;
				}
			}
		}
		else
		{
			unsigned long elemsPerThread = (image.m_imageData.size() / 4) / MAX_NUMBER_OF_THREADS;
			for (size_t thr = 0; thr < MAX_NUMBER_OF_THREADS; thr++)
			{
				unsigned rightLimit = (thr + 1) * elemsPerThread * 4;
				if (thr == MAX_NUMBER_OF_THREADS - 1)
					rightLimit = MAX_NUMBER_OF_THREADS * elemsPerThread * 4;
				unsigned leftLimit = thr * elemsPerThread * 4;
				std::jthread th([&, l = leftLimit, r = rightLimit, b = bitOffset]() {
					
					function(std::ref(image), l, r, b);
					
					});
				threads.push_back(std::move(th));
			}
		}
		for (auto& th : threads)
			th.join();
	}
}
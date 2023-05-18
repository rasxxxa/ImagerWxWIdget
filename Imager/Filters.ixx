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

	using MATRIX = std::vector<std::vector<RGB>>;

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

	void BlackAndWhite(Image& image, unsigned left, unsigned right, unsigned bitOffset)
	{
		auto& data = image.m_imageData;
		std::cout << &data << std::endl;
		for (unsigned pix = left; pix < right; pix += (4 * bitOffset))
		{
			unsigned int val = data[pix] + data[pix + 1] + data[pix + 2];

			val /= 3;

			unsigned char color = val < 128 ? 255 : 0;

			data[pix] = color;
			data[pix + 1] = color;
			data[pix + 2] = color;

			//data[pix + 3] = color;
		}
	}

	void Pixelate(Image& image, unsigned startI, unsigned endI, unsigned startJ, unsigned endJ, unsigned sampleSize, MATRIX& matrix)
	{
		for (unsigned i = startI; i < endI; i += sampleSize)
		{
			for (unsigned j = startJ; j < endJ; j += sampleSize)
			{
				unsigned long resultR = 0;
				unsigned long resultG = 0;
				unsigned long resultB = 0;
				unsigned long resultA = 0;

				for (unsigned k = i; k < endI && k < (k + sampleSize); k++)
				{
					for (unsigned l = j; l < endJ && l < (l + sampleSize); l++)
					{
						resultR += image.m_imageData[image.m_width * k + l];
						resultG += image.m_imageData[image.m_width * k + l + 1];
						resultB += image.m_imageData[image.m_width * k + l + 2];
						resultA += image.m_imageData[image.m_width * k + l + 3];
					}
				}

				resultR /= sampleSize;
				resultG /= sampleSize;
				resultB /= sampleSize;
				resultA /= sampleSize;

				for (unsigned k = i; k < endI && k < (k + sampleSize); k++)
				{
					for (unsigned l = j; l < endJ && l < (l + sampleSize); l++)
					{
						image.m_imageData[image.m_width * k + l    ] = resultR;
						image.m_imageData[image.m_width * k + l + 1] = resultG;
						image.m_imageData[image.m_width * k + l + 2] = resultB;
						image.m_imageData[image.m_width * k + l + 3] = resultA;
					}
				}

			}
		}
	}

	using MatrixFunc = std::function<void(Image&, unsigned, unsigned, unsigned, unsigned, unsigned, MATRIX&)>;
	using ArrayFunc = std::function<void(Image&, unsigned, unsigned, unsigned)>;

	void ThreadDivisionOfFunction(Image& image, bool useMatrixFormat, size_t bitOffset, const MatrixFunc& function, const ArrayFunc& arrayFunction)
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

			unsigned int perThreadI = matrix[0].size() / sqrt(MAX_NUMBER_OF_THREADS);
			unsigned int perThreadJ = matrix.size() / sqrt(MAX_NUMBER_OF_THREADS);

			for (unsigned int threadI = 0; threadI < sqrt(MAX_NUMBER_OF_THREADS); threadI++)
			{
				for (unsigned int threadJ = 0; threadJ < sqrt(MAX_NUMBER_OF_THREADS); threadJ++)
				{
					unsigned int startI, startJ, endI, endJ;
					startI = threadI * perThreadI;
					startJ = threadJ * perThreadJ;

					endI = (threadI + 1) * perThreadI;
					endJ = (threadJ + 1) * perThreadJ;

					if (threadI == 2)
						endI = matrix[0].size();

					if (threadJ == 2)
						endJ == matrix.size();

					std::jthread thread([&, i = startI, j = startJ, o = bitOffset, eI = endI, eJ = endJ]() 
						{
							function(std::ref(image), i, j, eI, eJ, o, std::ref(matrix));
						});
					threads.push_back(std::move(thread));
				}
			}
			for (auto& th : threads)
				th.join();

			for (unsigned i = 0; i < matrix.size(); i++)
			{
				for (unsigned j = 0; j < matrix[0].size(); j++)
				{
					auto elem = i * matrix.size() + j;
					image.m_imageData[elem] = matrix[i][j].R;
					image.m_imageData[elem + 1] = matrix[i][j].G;
					image.m_imageData[elem + 2] = matrix[i][j].B;
					image.m_imageData[elem + 3] = matrix[i][j].A;
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
					
					arrayFunction(std::ref(image), l, r, b);
					
					});
				threads.push_back(std::move(th));
			}
			for (auto& th : threads)
				th.join();
		}
	}
}
#include "Surface.h"
#include <cassert>
#include "ChiliWin.h"
#include <fstream>

Surface::Surface(const std::string filename)
{
	std::ifstream file(filename, std::ios::binary);
	assert(file);

	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*>( &bmFileHeader ), sizeof( bmFileHeader ) );

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert( bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32);
	assert( bmInfoHeader.biCompression == BI_RGB );

	const bool is32bit = bmInfoHeader.biBitCount == 32;

	width = bmInfoHeader.biWidth;

	//test to see if height is negitive 
	//reverse row according
	int yStart;
	int yEnd;
	int dy;

	if (bmInfoHeader.biHeight < 0)
	{
		height = -bmInfoHeader.biHeight;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		height = bmInfoHeader.biHeight;
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}

	pPixels = new Color[width * height];
	//padding is only for 24 bit 
	const int padding = (4 - (width * 3) % 4) % 4;

	for ( int y = yStart; y != yEnd; y += dy )
	{
		for ( int x = 0; x < width; ++x )
		{
			PutPixel(x, y, Color(file.get(), file.get(), file.get()));
			if (is32bit)
			{
				file.seekg(1, std::ios::cur);
			}
		}
		if (!is32bit)
		{
			file.seekg(padding, std::ios::cur);
		}
	}
} 

Surface::Surface(int width_in, int height_in)
	:
	width( width_in ),
	height( height_in ),
	pPixels( new Color[width_in * height_in] )
{
}

Surface::Surface(const Surface& rhs)
	:
	Surface( rhs.GetWidth(), rhs.GetHeight() )
{
	int totalPixels = width * height;
	for (int i = 0; i < totalPixels; ++i)
	{
		pPixels[i] = rhs.pPixels[i];
	}
}

Surface::~Surface()
{
	delete[] pPixels;
	pPixels = nullptr;
}

Surface& Surface::operator=(const Surface & rhs)
{
	width = rhs.width;
	height = rhs.height;
	int totalPixels = width * height;

	delete[] pPixels;
	pPixels = new Color[totalPixels];

	for (int i = 0; i < totalPixels; ++i)
	{
		pPixels[i] = rhs.pPixels[i];
	}

	return *this;
}

void Surface::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < width);
	assert(y < height);
	pPixels[width * y + x] = c;
}

Color Surface::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < width);
	assert(y < height);
	return pPixels[width * y + x];
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

RectI Surface::GetRect() const
{
	return {0,width,0,height};
}

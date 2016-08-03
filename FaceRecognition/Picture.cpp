#include "stdafx.h"
#include "Picture.h"

static void SaveBmp(char* data, int width, int height);

Picture::Picture(const char* data, size_t len)
:buffer(data,len)
{
}

Picture::~Picture()
{
	out2bmp();
}

size_t Picture::width() const
{
	return _width;
}

size_t Picture::height() const
{
	return _height;
}

void Picture::SetWidth(size_t width)
{
	_width = width;
}

void Picture::SetHeight(size_t height)
{
	_height = height;
}

void Picture::out2bmp()
{
	SaveBmp(buffer.begin(),_width, _height);
}

#include <Poco/TemporaryFile.h>
#include <Poco/FileStream.h>

using Poco::TemporaryFile;
using Poco::FileOutputStream;

void SaveBmp(char* data, int width, int height)
{
	BITMAPINFOHEADER bih;
	bih.biSize = 40; 						// header size
	bih.biWidth = width;
	bih.biHeight = height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;					// RGB encoded, 24 bit
	bih.biCompression = BI_RGB;			// no compression
	bih.biSizeImage = width*height * 3;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;

	BITMAPINFO bi;
	memset(&bi, 0, sizeof(bi));
	memcpy(&(bi.bmiHeader), &bih, sizeof(BITMAPINFOHEADER));
	int iWidth = bih.biWidth;
	int iHeight = bih.biHeight;

	DWORD word = 0;
	std::string Path = "d:\\bmp\\";
	std::string name = "test";
	std::string ext = ".bmp";
	name += std::to_string(::GetTickCount()).c_str();
	Path.append(name);
	Path.append(ext);

	TemporaryFile file(Path);
	FileOutputStream fs(Path);
	//fileheader
	BITMAPFILEHEADER bmfh;
	int nBitsOffset = sizeof(BITMAPFILEHEADER)+bi.bmiHeader.biSize;
	LONG lImageSize = bi.bmiHeader.biSizeImage;
	LONG lFileSize = nBitsOffset + lImageSize;
	bmfh.bfType = 'B' + ('M' << 8);
	bmfh.bfOffBits = nBitsOffset;
	bmfh.bfSize = lFileSize;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	fs.write((char*)&bmfh, sizeof(BITMAPFILEHEADER));
	//header
	fs.write((char*)&bi.bmiHeader, sizeof(BITMAPINFOHEADER));
	//data
	fs.write((char*)data, bih.biSizeImage);
	fs.close();
}
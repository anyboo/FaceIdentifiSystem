#include "stdafx.h"
#include "Picture.h"

#include "log.h"


static std::string SaveBmp(char* data, int width, int height);


Picture::Picture(const Picture& pic)
:buffer(pic.data(),pic.len())
{
}

Picture::Picture()
:buffer(512)
{

}

Picture::Picture(const char* data, size_t len)
:buffer(data,len)
{
}

Picture::~Picture()
{
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
	SaveBmp(buffer.begin(),_width, _height, _T("D:\\bmp\\test.bmp"));
}

std::string Picture::out2bmp(const std::string& path)
{
	return SaveBmp(buffer.begin(), _width, _height ,path);
}

std::string Picture::WriteToDisk()
{
	return SaveBmp(buffer.begin(), _width, _height, _T("D:\\bmp\\test.bmp"));
}

const char* Picture::data() const
{
	return buffer.begin();
}

const size_t Picture::len() const
{
	return buffer.size();
}

void Picture::MirrorDIB(bool bDirection, int nImageBits)
{
	Picture::MirrorDIB(data(), _width, _height, bDirection, nImageBits);
}

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
void Picture::MirrorDIB(const char* lpDIBBits, long lWidth, long lHeight, bool bDirection, int nImageBits)
{
	LPSTR	lpSrc;
	LPSTR	lpDst;
	LPSTR	lpBits;
	HLOCAL	hBits;
	LONG	i;
	LONG	j;
	int nBits;
	LONG lLineBytes;
	lLineBytes = WIDTHBYTES(lWidth *nImageBits);
	hBits = LocalAlloc(LHND, lLineBytes);
	if (hBits == NULL) return;

	lpBits = (char *)LocalLock(hBits);
	int nStep = nImageBits / 8;
	long lCenter = lWidth / 2 * nStep;

	if (bDirection)
	{
		for (i = 0; i < lHeight; i++)
		{
			for (j = 0; j < lCenter; j += nStep)
			{
				for (nBits = 0; nBits<nStep; nBits++)
				{
					lpSrc = (char *)lpDIBBits + lLineBytes * i + lCenter - j + nBits;
					lpDst = (char *)lpDIBBits + lLineBytes * i + lCenter + j + nBits;
					*lpBits = *lpDst;
					*lpDst = *lpSrc;
					*lpSrc = *lpBits;
				}

			}

		}
	}
	else
	{
		for (i = 0; i < lHeight / 2; i++)
		{
			lpSrc = (char *)lpDIBBits + lLineBytes * i;
			lpDst = (char *)lpDIBBits + lLineBytes * (lHeight - i - 1);
			memcpy(lpBits, lpDst, lLineBytes);
			memcpy(lpDst, lpSrc, lLineBytes);
			memcpy(lpSrc, lpBits, lLineBytes);

		}
	}
	LocalUnlock(hBits);
	LocalFree(hBits);
}

#include <Poco/TemporaryFile.h>
#include <Poco/FileStream.h>

using Poco::TemporaryFile;
using Poco::FileOutputStream;


std::string Picture::SaveBmp(char* data, int width, int height, const std::string& path)
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
	std::string Path = path;
	std::string name = "test";
	std::string ext = ".bmp";
	//	name += std::to_string(::GetTickCount()).c_str();
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

	return Path;
}
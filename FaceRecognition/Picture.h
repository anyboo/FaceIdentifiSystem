#pragma once

#include <Poco/AutoPtr.h>
#include <Poco/Buffer.h>
#include <Poco/RefCountedObject.h>

class Picture
	:public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<Picture> Ptr;

	Picture(size_t width, size_t height, size_t bit);
	Picture(const Picture& pic);
	Picture& operator = (const Picture& other);

	size_t width() const;
	size_t height() const;

	char* data();
	size_t len();

	void MirrorDIB(bool bDirection, int nImageBits = 24);
	static void MirrorDIB(const char* lpDIBBits, long lWidth, long lHeight, bool bDirection, int nImageBits);
	void out2bmp();

	std::string out2bmp(const std::string& path);

	std::string WriteToDisk();

	virtual ~Picture();
protected:
	Picture();
	std::string SaveBmp(char* data, int width, int height, const std::string& path);
	void swap(const Picture& other);
private:
	
	Poco::Buffer<char> buffer;

	size_t _width;
	size_t _height;
	size_t _bit;
};



#pragma once

#include <Poco/AutoPtr.h>
#include <Poco/Buffer.h>
#include <Poco/RefCountedObject.h>

class Picture
	:public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<Picture> Ptr;

	Picture(const char* data, size_t len);
	Picture(const Picture& pic);

	size_t width() const;
	size_t height() const;

	const char* data() const;
	const size_t len() const;

	void SetWidth(size_t width);
	void SetHeight(size_t height);

	void MirrorDIB(bool bDirection, int nImageBits = 24);
	static void MirrorDIB(const char* lpDIBBits, long lWidth, long lHeight, bool bDirection, int nImageBits);
	void out2bmp();
	std::string out2bmp(const std::string& path);
	virtual ~Picture();
protected:
	Picture();
	std::string SaveBmp(char* data, int width, int height, const std::string& path);
private:

	Poco::Buffer<char> buffer;

	size_t _width;
	size_t _height;
};



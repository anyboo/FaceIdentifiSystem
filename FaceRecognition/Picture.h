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
	~Picture();

	size_t width() const;
	size_t height() const;

	void SetWidth(size_t width);
	void SetHeight(size_t height);

protected:
	void out2bmp();
private:
	Poco::Buffer<char> buffer;

	size_t _width;
	size_t _height;
};



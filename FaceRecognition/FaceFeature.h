#pragma once

#include <Poco/AutoPtr.h>
#include <Poco/Buffer.h>
#include <Poco/RefCountedObject.h>

class FaceImage;
class FacePosition;

class FaceFeature
	:public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<FaceFeature> Ptr;
	FaceFeature();
	~FaceFeature();
	void Extract(FaceImage* image, FacePosition* fpos);
	const char* data();
private:
	Poco::Buffer<char> _data;
	short ChannelID;
	const short magic = 3;
	const int success = 1;
};


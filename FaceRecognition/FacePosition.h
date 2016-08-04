#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Buffer.h>
#include <Poco/AutoPtr.h>
#include <Poco/RefCountedObject.h>

class FaceImage;
class THFI_FacePos;

class FacePosition
	:public Poco::RefCountedObject
{
public:
	typedef Poco::AutoPtr<FacePosition> Ptr;
	FacePosition();
	~FacePosition();

	size_t DetectFace(FaceImage* image);
	unsigned long facial() const;
private:
	Poco::SharedPtr<THFI_FacePos> _position;
	Poco::Buffer<char> _Reserved;
	short ChannelID;
	int maxFaceCount;
};


#pragma once

#include "Picture.h"
#include "FaceFeature.h"
#include "FacePosition.h"
#include <Poco/Stopwatch.h>

class FaceFeature;
class FacePosition;

class FaceImage
	: public Picture
{
public:
	typedef Poco::AutoPtr<FaceImage> Ptr;

	FaceImage(Picture::Ptr pic);
	FaceImage(Picture::Ptr pic, int width, int height);
	FaceImage(FaceImage& image);
	bool Compare(FaceImage& image);
	int bpp()const;

	void SaveImageData();

protected:
	FaceImage();
	bool IsFace();

	void DetectFace();
	void Extract();

private:
	int _bpp;
	size_t detectedFaceCount;
	FacePosition::Ptr fpos;
	FaceFeature::Ptr feature;
};


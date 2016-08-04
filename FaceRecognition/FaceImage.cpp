#include "stdafx.h"
#include "FaceImage.h"
#include "FacePosition.h"
#include "FaceFeature.h"
#include "THFeature_i.h"
FaceImage::FaceImage()
: _bpp(24), fpos(new FacePosition), feature(new FaceFeature)
{
	poco_assert(0);	
}

FaceImage::FaceImage(Picture::Ptr pic)
: Picture(*pic), _bpp(24), fpos(new FacePosition), feature(new FaceFeature)
{
	this->SetHeight(pic->height());
	this->SetWidth(pic->width());
	Picture::MirrorDIB(data(), width(), height(), false, 24);	
	if (DetectFace() > 0)
		Extract();
}

FaceImage::FaceImage(const FaceImage& image)
:Picture(image.data(),image.len()), _bpp(24)
, fpos(new FacePosition), feature(new FaceFeature)
{
	Picture::MirrorDIB(data(), width(), height(), false, 24);	
	if (DetectFace() > 0)
		Extract();
}

bool FaceImage::Compare(FaceImage& image)
{
	BYTE* data1 = (BYTE*)feature->data();
	BYTE* data2 = (BYTE*)(image.feature)->data();
	float similarity = EF_Compare(data1, data2);
	OutputDebugStringA((std::string("similarity:") + std::to_string(similarity)).c_str());
	return (similarity >= 0.6);
}

int FaceImage::DetectFace()
{
	poco_check_ptr(fpos);
	detectedFaceCount = fpos->DetectFace(this);
	//only process one face
	if (detectedFaceCount <= 0)
	{
		OutputDebugStringA((std::string("Not Face Detected!") + std::to_string(detectedFaceCount)).c_str());
		throw std::exception("Not Face Detected!");
	}
	return detectedFaceCount;
}

void FaceImage::Extract()
{
	feature->Extract(this, fpos);
}

int FaceImage::bpp()const
{
	return _bpp;
}


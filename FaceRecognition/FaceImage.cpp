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
	DetectFace();
	Extract();
}

FaceImage::FaceImage(Picture::Ptr pic, int picwidth, int picheight)
	: Picture(*pic), _bpp(24), fpos(new FacePosition), feature(new FaceFeature)
{
	this->SetHeight(picheight);
	this->SetWidth(picwidth);
	Picture::MirrorDIB(data(), width(), height(), true, 24);
	Picture::MirrorDIB(data(), width(), height(), false, 24);
	DetectFace();
	Extract();
}

FaceImage::FaceImage(FaceImage& image)
:Picture(image.data(),image.len()), _bpp(24)
, fpos(new FacePosition), feature(new FaceFeature)
{
	Picture::MirrorDIB(data(), width(), height(), false, 24);	
	DetectFace();
	Extract();
}

#include <sstream>

bool FaceImage::Compare(FaceImage& image)
{
	BYTE* data1 = (BYTE*)feature->data();
	BYTE* data2 = (BYTE*)(image.feature)->data();
	float similarity = EF_Compare(data1, data2);
	std::stringstream ostr;
	ostr << "similarity:" << similarity << std::endl;
	OutputDebugStringA(ostr.str().c_str());
	return (similarity >= 0.6);
}

#include <sstream>
void FaceImage::DetectFace()v
{
	poco_check_ptr(fpos);
	detectedFaceCount = fpos->DetectFace(this);
	//only process one face
	if (detectedFaceCount <= 0)
	{
		std::stringstream ostr;
		ostr << "Not Face Detected! - " << detectedFaceCount << std::endl;
		OutputDebugStringA(ostr.str().c_str());
		throw std::exception("Not Face Detected!");
	}
}

void FaceImage::Extract()
{
	poco_check_ptr(feature);
	feature->Extract(this, fpos);
}

int FaceImage::bpp()const
{
	return _bpp;
}


#include "stdafx.h"
#include "FaceImage.h"
#include "FacePosition.h"
#include "FaceFeature.h"
#include "THFeature_i.h"
#include "log.h"

#include "SettingConfig.h"
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
	Poco::Stopwatch sw;
	sw.start();
	float similarity = EF_Compare(data1, data2);
	sw.stop();
	//OutputDebugStringA(ostr.str().c_str());
	poco_information_f2(logger_handle, "Compare :%Ld similarity : %f", Poco::Clock(sw.elapsed()).microseconds(), similarity);
	ValueSetting vset;
	float leastSiml = vset.SetSimilarity();
	return (similarity >= leastSiml);
}

#include <sstream>
void FaceImage::DetectFace()
{
	poco_check_ptr(fpos);
	Poco::Stopwatch sw;
	sw.start();
	detectedFaceCount = fpos->DetectFace(this);
	sw.stop();
	//OutputDebugStringA(ostr.str().c_str());
	poco_information_f1(logger_handle, "DetectFace elapsed: %Ld", Poco::Clock(sw.elapsed()).microseconds());
	//only process one face
	if (detectedFaceCount <= 0)
	{
		/*std::stringstream ostr;
		ostr << "Not Face Detected! - " << detectedFaceCount << std::endl;*/
		//OutputDebugStringA(ostr.str().c_str());
		throw std::exception("Not Face Detected!");
	}
}

void FaceImage::Extract()
{
	poco_check_ptr(feature);
	Poco::Stopwatch sw;
	sw.start();
	feature->Extract(this, fpos);
	sw.stop();
	poco_information_f1(logger_handle, "Extract elapsed: %Ld", Poco::Clock(sw.elapsed()).microseconds());
}

int FaceImage::bpp()const
{
	return _bpp;
}


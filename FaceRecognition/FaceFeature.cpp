#include "stdafx.h"
#include "FaceFeature.h"
#include "FaceImage.h"
#include "FacePosition.h"
#include "THFeature_i.h"

FaceFeature::FaceFeature()
:_data(0), ChannelID(0)
{
}

FaceFeature::~FaceFeature()
{
}

void FaceFeature::Extract(FaceImage* image, FacePosition* fpos)
{
	DWORD FacialData = fpos->facial();
	DWORD feature_size = EF_Size();
	BYTE* feature = new BYTE[feature_size];
	//If the function succeeds, the return value is 1
	int ret = -1;
	if (ret = EF_Extract(ChannelID, (BYTE*)image->data(), image->width(), image->height(),
		magic, FacialData, feature) != success)
	{
		delete[] feature;
		throw std::exception("Extract failed!");
		//OutputDebugStringA((std::string("Extract failed! ret:") + std::to_string(ret)).c_str());
	}	
	_data.assign((const char*)feature, feature_size);
	delete[] feature;
}

const char* FaceFeature::data()
{
	return _data.begin();
}
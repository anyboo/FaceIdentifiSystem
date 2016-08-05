#include "stdafx.h"
#include "FacePosition.h"
#include "THFaceImage_i.h"
#include "FaceImage.h"

FacePosition::FacePosition()
:_position(new THFI_FacePos), _Reserved(512)
, maxFaceCount(1), ChannelID(0)
{
	_position->dwReserved = (unsigned long)_Reserved.begin();
}


FacePosition::~FacePosition()
{
}


size_t FacePosition::DetectFace(FaceImage* image)
{
	int face_number = THFI_DetectFace(ChannelID, (BYTE*)image->data(),
					image->bpp(), image->width(), image->height(),
					_position, maxFaceCount);
	
	return ((face_number > 0) ? face_number : 0);
}

unsigned long FacePosition::facial() const
{
	return (unsigned long)(THFI_FacePos*)_position.get();
}
#pragma once
#include "TiCapture2.h"
#include "THFaceImage_i.h"
#include "THFeature_i.h"

class CInitDevice
{
public:
	CInitDevice();
	~CInitDevice();
	void InitStart();
public:
	CCapture* m_pCap;
	SCapParam m_capParam;
	int m_iFrame;
	long m_nFeatureSize;
	BYTE* m_pEnrollFeature;
};


#pragma once

#include "Poco/Runnable.h"
#include "Poco/Thread.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "THFaceImage_i.h"
#include "THFeature_i.h"

class BitMapCompare : public Poco::Runnable
{
public:
	void run();
private:
	void CompareBitmap(BYTE *pFirst, BYTE *pSecond, long nFirstWidth, long nSecondWidth, long nFirstHeight, long nSecondHeight, float& fRet);
	cv::Mat LoadBmpFile1(std::string strFilePath);
	void getCamBuf();
	void writeDB();
	void onEvent(const void* pSender, bool& arg);

private:
	bool             _break;
};
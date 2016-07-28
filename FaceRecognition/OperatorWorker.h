#ifndef __OPERATORWORKER_H__
#define __OPERATORWORKER_H__

#include <iostream>
#include <queue>

#include "TiCapture2.h"
#include "THFaceImage_i.h"
#include "THFeature_i.h"

#include "Picture.h"

#include "Poco/Activity.h"
#include "Poco/Thread.h"

using Poco::Activity;
using Poco::Thread;

class OperatorWorker
{
public:
	OperatorWorker();
	~OperatorWorker();

	void InitCapture();
	void startCapture();
	void stopCapture();

	void start();
	void stop();

	void getPhotography();

	queue<Picture> getDataQueue()const;

private:
 	BOOL MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection, int nImageBits);
 	void ContructBih(int nWidth, int nHeight, BITMAPINFOHEADER& bih);
private:
	CCapture* m_pCap;
	SCapParam m_capParam;
	//int m_iFrame;
	long m_nFeatureSize;
	std::queue<Picture> m_DataQueue;
	
	Activity<OperatorWorker> _activity;
};

#endif 

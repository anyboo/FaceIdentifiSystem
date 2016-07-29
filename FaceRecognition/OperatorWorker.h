#ifndef __OPERATORWORKER_H__
#define __OPERATORWORKER_H__

#include <atlstr.h>
#include <windows.h>
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

	
	void SaveBmp(BITMAPINFO& bi, BYTE* pDataBuf);

	void SetHandle(HWND hWnd);
	queue<Picture> getDataQueue()const;

private:
	void getPhotography();

 	BOOL MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection, int nImageBits);
 	void ContructBih(int nWidth, int nHeight, BITMAPINFOHEADER& bih);
	void DrawBmpBuf(BITMAPINFOHEADER& bih, BYTE* pDataBuf, HWND hShowWnd, BOOL bFitWnd);
private:
	CCapture* m_pCap;
	SCapParam m_capParam;
	//int m_iFrame;
	long m_nFeatureSize;
	std::queue<Picture> m_DataQueue;
	
	Activity<OperatorWorker> _activity;
	HWND m_hWnd;
};

#endif 

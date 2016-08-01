#pragma once

#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "QMFileSqlite.h"

class BitMapCompare : public Poco::Runnable
{
public:
	BitMapCompare(void *pthis);
	~BitMapCompare();
	void run();
private:
	void CompareBitmap(BYTE *pFirst, BYTE *pSecond, long nFirstWidth, long nSecondWidth, long nFirstHeight, long nSecondHeight, float& fRet);
	Mat LoadBmpFile1(std::string strFilePath);
	void getCamBuf();
	void writeDB();
	void onEvent(const void* pSender, bool& arg);
	void getUserInfo();

private:
	bool             _break;
	void *           _pWnd;
	vector<readUserInfo> _vUserInfo;
};
#include "stdafx.h"
#include "BitMapCompare.h"
#include <queue>
#include "MonitoringUI.h"

using namespace cv;

BitMapCompare::BitMapCompare(void *pthis)
{
	_pWnd = pthis;
}

BitMapCompare::~BitMapCompare()
{

}

void BitMapCompare::CompareBitmap(BYTE *pFirst, BYTE *pSecond, long nFirstWidth, long nSecondWidth, long nFirstHeight, long nSecondHeight, float& fRet)
{
	long t_start, t_end;
	t_start = GetTickCount();
	int k;
	//face detect
	THFI_FacePos ptfp1[1];
	BYTE* pFeature1 = NULL;
	BYTE* pFeature2 = NULL;
	
	for (k = 0; k<1; k++)
	{
		ptfp1[k].dwReserved = (DWORD)new BYTE[512];
	}
	//获取面部
	int nNum1 = THFI_DetectFace(0, pFirst, 24, nFirstWidth, nFirstHeight, ptfp1, 1);//only process one face

	if (nNum1 > 0)
	{
		//第一副照片需要比较的内容
		pFeature1 = new BYTE[EF_Size()];

		//only extract the first face(max size face)获取面部信息
		int ret = EF_Extract(0, pFirst, nFirstWidth, nFirstHeight, 3, (DWORD)&ptfp1[0], pFeature1);
		if (ret)
		{

		}
		else
		{
			delete[]pFeature1;
			pFeature1 = NULL;
		}
		for (k = 0; k<1; k++)
		{
			delete[](BYTE*)ptfp1[k].dwReserved;
		}	
	}

	//face detect获取面部信息
	THFI_FacePos ptfp2[1];
	for (k = 0; k<1; k++)
	{
		ptfp2[k].dwReserved = (DWORD)new BYTE[512];
	}
	int nNum2 = THFI_DetectFace(0, pSecond, 24, nSecondWidth, nSecondHeight, ptfp2, 1);
	if (nNum2 > 0)
	{
		pFeature2 = new BYTE[EF_Size()];

		//only extract the first face(max size face)获取比较内容
		int ret = EF_Extract(0, pSecond, nSecondWidth, nSecondHeight, 3, (DWORD)&ptfp2[0], pFeature2);
		if (ret)
		{

		}
		else
		{
			delete[]pFeature2;
			pFeature2 = NULL;
		}

		for (k = 0; k<1; k++)
		{
			delete[](BYTE*)ptfp2[k].dwReserved;
		}
	}
	
	if ((nNum1 > 0) && (nNum2 > 0))
	{
		float score = 0.0f;
		score = EF_Compare(pFeature1, pFeature2);
		fRet = score;
		t_end = GetTickCount();
		char sztmp[50] = { 0 };
		sprintf_s(sztmp, "compare result:%f, time: %d", score, t_end - t_start);
		std::cout << sztmp << std::endl;
	}
	else
	{
		char sztmp[50] = { 0 };
		sprintf_s(sztmp, "first face num:%d, second face num:%d", nNum1, nNum2);
		std::cout << sztmp << std::endl;
	}
}

Mat BitMapCompare::LoadBmpFile1(std::string strFilePath)
{
	Mat im = imread(strFilePath);
	return im;
}

void BitMapCompare::run()
{
	for (;;)
	{
		if (_break)
		{
			std::cout << "listen is break" << std::endl;
			break;
		}
		//读取数据库内注册数据		
		getUserInfo();

		//获取摄像头数据

		//开始比对


		//将大于0.6的数据写入数据库
		CMonitoringUI *pWnd = (CMonitoringUI *)_pWnd;
		std::queue<readCompareInfo> pcompare = pWnd->getCompareQueue();
		readCompareInfo rCompareInfo;
		pcompare.push(rCompareInfo);
		
		Poco::Thread::sleep(200);
	}
}

void BitMapCompare::onEvent(const void* pSender, bool& arg)
{
	std::cout << "arg : " << arg << std::endl;
	_break = arg;
}

void BitMapCompare::getUserInfo()
{
	QFileSqlite *pDb = QFileSqlite::getInstance();
	vector<int> vUserCount;
	pDb->GetData(SELECT_COUNT_USER_INFO, vUserCount);
	if (vUserCount[0] != _vUserInfo.size())
	{
		pDb->GetData(SELECT_ALL_USER_INFO, _vUserInfo);
	}	
}

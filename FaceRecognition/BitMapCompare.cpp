

#include "stdafx.h"
#include "BitMapCompare.h"

void BitMapCompare::CompareBitmap(BYTE *pFirst, BYTE *pSecond, long nFirstWidth, long nSecondWidth, long nFirstHeight, long nSecondHeight, float& fRet)
{
	long t_start, t_end;
	t_start = GetTickCount();
	int k;
	//face detect
	THFI_FacePos ptfp1[1];

	for (k = 0; k<1; k++)
	{
		ptfp1[k].dwReserved = (DWORD)new BYTE[512];
	}
	//��ȡ�沿
	int nNum1 = THFI_DetectFace(0, pFirst, 24, nFirstWidth, nFirstHeight, ptfp1, 1);//only process one face

	//RECT rcFace = ptfp1[0].rcFace;
	//��һ����Ƭ��Ҫ�Ƚϵ�����
	BYTE* pFeature1 = new BYTE[EF_Size()];

	//only extract the first face(max size face)��ȡ�沿��Ϣ
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

	//face detect��ȡ�沿��Ϣ
	THFI_FacePos ptfp2[1];
	for (k = 0; k<1; k++)
	{
		ptfp2[k].dwReserved = (DWORD)new BYTE[512];
	}
	int nNum2 = THFI_DetectFace(0, pSecond, 24, nSecondWidth, nSecondHeight, ptfp2, 1);
	//rcFace = ptfp2[0].rcFace;

	BYTE* pFeature2 = new BYTE[EF_Size()];

	//only extract the first face(max size face)��ȡ�Ƚ�����
	ret = EF_Extract(0, pSecond, nSecondWidth, nSecondHeight, 3, (DWORD)&ptfp2[0], pFeature2);
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

void run()
{
	for (;;)
	{
		//��ȡ���ݿ���ע������


		//��ȡ����ͷ����


		//��ʼ�ȶ�


		//������0.6������д�����ݿ�

	}
}
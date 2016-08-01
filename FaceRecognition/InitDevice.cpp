#include "stdafx.h"
#include "InitDevice.h"


CInitDevice::CInitDevice()
{
}


CInitDevice::~CInitDevice()
{
}

void CInitDevice::InitStart()
{
	THFI_Param param;

	param.nMinFaceSize = 150;
	param.nRollAngle = 145;
	param.bOnlyDetect = true;
	THFI_Create(1, &param);

	short ret = EF_Init(1);
	if (ret == 1)
	{
		//AfxMessageBox("Feature init ok");
	}

	m_nFeatureSize = EF_Size();
	m_pEnrollFeature = new BYTE[m_nFeatureSize];

	CWaitCursor cursor;

	m_capParam.szCap.cx = 400;
	m_capParam.szCap.cy = 420;
	m_capParam.eType = CAP_WDM;
	m_capParam.lIndex = 0;
	m_pCap = CCapture::Create(&m_capParam);

	if (NULL == m_pCap)
		return;
	
	BOOL bOK = m_pCap->InitCapture();
	if (!bOK)
	{
		delete m_pCap;
		m_pCap = NULL;
		return;
	}

	m_pCap->Play();
}

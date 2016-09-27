#pragma once
#include <DuiLib/UIlib.h>

class CDeviceManage
	:public CNotifyPump
{
public:
	CDeviceManage();
	~CDeviceManage();

	void SetPaintMagager(CPaintManagerUI* pPaintMgr);

	void InitDeviceInfo();

	void SaveDeviceInfo();
	void ReadDeviceInfo();

	DUI_DECLARE_MESSAGE_MAP();


private:
	CPaintManagerUI* _ppm;

};



#pragma once
#include <DuiLib/UIlib.h>

class DeviceUI :
	public DuiLib::CContainerUI
{
public:
	DeviceUI()
	{
		DuiLib::CDialogBuilder builder;
		CContainerUI* pDeviceUI = static_cast<CContainerUI*>(builder.Create(_T("xml/Device.xml"), (UINT)0));
		if (pDeviceUI) {
			this->Add(pDeviceUI);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};
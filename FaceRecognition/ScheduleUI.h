#pragma once
#include <DuiLib/UIlib.h>

class ScheduleUI :
	public DuiLib::CContainerUI
{
public:
	ScheduleUI()
	{
		DuiLib::CDialogBuilder builder;
		CContainerUI* pUI = static_cast<CContainerUI*>(builder.Create(_T("xml/Schedule.xml"), (UINT)0));
		if (pUI) {
			this->Add(pUI);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};
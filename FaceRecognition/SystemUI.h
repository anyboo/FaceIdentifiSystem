#pragma once
#include <DuiLib/UIlib.h>

class SystemUI :
	public DuiLib::CContainerUI
{
public:
	SystemUI()
	{
		DuiLib::CDialogBuilder builder;
		CContainerUI* pUI = static_cast<CContainerUI*>(builder.Create(_T("xml/System.xml"), (UINT)0));
		if (pUI) {
			this->Add(pUI);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};
#pragma once
#include <DuiLib/UIlib.h>

class UserUI :
	public DuiLib::CContainerUI
{
public:
	UserUI()
	{
		DuiLib::CDialogBuilder builder;
		CContainerUI* pUI = static_cast<CContainerUI*>(builder.Create(_T("xml/User.xml"), (UINT)0));
		if (pUI) {
			this->Add(pUI);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};
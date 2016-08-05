#pragma once
#include "Picture.h"
#define  DUI_CTR_CLIP                         (_T("Clip"))

class CClipUI 
	: public CControlUI
{
public:
	CClipUI();
	~CClipUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
private:
	Picture::Ptr _picture;
};

